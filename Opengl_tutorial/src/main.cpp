#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "graphic/shader.hpp"
#include "io/keyboard.hpp"
#include "io/mouse.hpp"
#include "io/joystick.hpp"
#include "io/camera.hpp"
#include "io/screen.hpp"
#include "graphic/texture.hpp"
#include <iostream>

void processInput(float delta);

// settings

float mixVal = 0.5f;

glm::mat4 transform = glm::mat4(1.0f);

Joystick mainJ(0);

Camera cameras[] = {
    Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
    Camera(glm::vec3(0.0f, 0.0f, 7.0f)),
};

Screen screen;

int activeCamera = 0;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    float lastTime = glfwGetTime();
    // glfw window creation
    // --------------------
    if(!screen.init()) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    screen.setParamters();
    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/object.vs.glsl", "/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/object.fs.glsl");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute

    Texture texture1("/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/image1.jpeg", "texture1", true);
    texture1.load();
    Texture texture2("/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/image2.png", "texture2", true);
    texture2.load();
    
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.activate(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(ourShader.id, "texture1"), 0);
    // or set it via the texture class
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    
    if(mainJ.isPresent()) {
        mainJ.update();
        std::cout << "joystick is preseted!" << std::endl;
    }

    // render loop
    // -----------
    while (!screen.shouldClose())
    {
        float currentTime = glfwGetTime();
        float delta = currentTime - lastTime;
        lastTime = currentTime;
        // input
        // -----
        processInput(delta);
        // render
        // ------
        screen.update();
        // bind textures on corresponding texture units
        texture1.activate();
        glBindTexture(GL_TEXTURE_2D, texture1.id);
        texture2.activate();
        glBindTexture(GL_TEXTURE_2D, texture2.id);
        glBindVertexArray(VAO);
        
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        
        model = glm::rotate(model,(float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.5f));
        view = cameras[activeCamera].getViewMatrix();
        projection = glm::perspective(cameras[activeCamera].getZoom(),(float)Screen::SCR_WIDTH/(float)Screen::SCR_HEIGHT, 0.1f, 100.0f);
        // render container
        ourShader.activate();
        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        ourShader.setFloat("mixVal", mixVal);
        transform = glm::rotate(transform, glm::radians(0.5f), glm::vec3(1.0, 0.0f, 0.0f));
        ourShader.setMat4("transform", transform);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        screen.newFrame();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(float delta)
{
    if(Keyboard::keyWentUp(GLFW_KEY_ESCAPE)) {
        screen.setShouldClose(true);
    } else if(Keyboard::keyWentUp(GLFW_KEY_UP)) {
        mixVal += 0.05f;
        if(mixVal > 1.0f) {
            mixVal = 1.0f;
        }
    } else if(Keyboard::keyWentUp(GLFW_KEY_DOWN)) {
        mixVal -= 0.05f;
        if(mixVal < 0.0f) {
            mixVal = 0.0f;
        }
    } else if(Keyboard::key(GLFW_KEY_W)) {
        cameras[activeCamera].updateCameraPos(CameraDirection::UP, delta);
    } else if(Keyboard::key(GLFW_KEY_S)) {
        cameras[activeCamera].updateCameraPos(CameraDirection::DOWN, delta);
    } else if(Keyboard::key(GLFW_KEY_A)) {
        cameras[activeCamera].updateCameraPos(CameraDirection::LEFT, delta);
    } else if(Keyboard::key(GLFW_KEY_D)) {
        cameras[activeCamera].updateCameraPos(CameraDirection::RIGHT, delta);
    } else if(Keyboard::keyWentDown(GLFW_KEY_TAB)) {
        activeCamera += activeCamera == 0 ? 1 : -1;
    }
    
    double dx = Mouse::getDX(), dy = Mouse::getDY();
    if(dx != 0 && dy != 0) {
        cameras[activeCamera].updateCameraDirection(dx/10, dy/10);
    }

//    double scrollDy = Mouse::getScrollDY();
//    if(scrollDy != 0) {
//        std::cout << scrollDy << std::endl;
//        camera.updateCameraZoom(scrollDy);
//    }
    
    mainJ.update();
    
//    float lx = mainJ.axesState(GLFW_JOYSTICK_AXES_LEFT_STICK_X);
//    float ly = mainJ.axesState(GLFW_JOYSTICK_AXES_LEFT_STICK_Y);
//    if(std::abs(lx) > 0.05f) {
//        transform = glm::translate(transform, glm::vec3(lx/10, 0.0f, 0.0f));
//    }
//    if(std::abs(ly) > 0.05f) {
//        transform = glm::translate(transform, glm::vec3(0.0f, ly/10, 0.0f));
//    }
//    float rt = mainJ.axesState(GLFW_JOYSTICK_AXES_RIGHT_TRIGGER) / 2 + 0.5;
//    if(std::abs(rt) > 0.05f) {
//        transform = glm::scale(transform, glm::vec3(1 + rt/10, 1 + rt/10, 0.0f));
//    }
//    float lt = mainJ.axesState(GLFW_JOYSTICK_AXES_LEFT_TRIGGER) / 2 + 0.5;
//    if(std::abs(lt) > 0.05f) {
//        transform = glm::scale(transform, glm::vec3(1 + lt/10, 1 + lt/10, 0.0f));
//    }
//    std::cout << lx << ":" << ly << std::endl;
}
