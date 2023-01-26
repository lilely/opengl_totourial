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
#include "graphic/models/cube.hpp"
#include "graphic/material.hpp"
#include "graphic/models/lamp.hpp"

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
    Shader ourShader("/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/object.vs.glsl", "/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/object_texture.fs.glsl");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    Shader lampShader("/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/object.vs.glsl","/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/lamp.fs.glsl");
    
    Cube cube(Material::emerald,glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.75f),"box.png","box_specular.png");
    cube.init();
    
    Lamp lamp(glm::vec3(1.0f),glm::vec3(1.0f),glm::vec3(1.0f),glm::vec3(1.0f),glm::vec3(-1.0f,-0.5f,-0.5f), glm::vec3(0.25f));
    lamp.init();
    
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
        
        ourShader.activate();
        ourShader.setFloat3("light.position", lamp.pos);
        ourShader.setFloat3("viewPos", cameras[activeCamera].cameraPos);
        
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        
        view = cameras[activeCamera].getViewMatrix();
        projection = glm::perspective(cameras[activeCamera].getZoom(),(float)Screen::SCR_WIDTH/(float)Screen::SCR_HEIGHT, 0.1f, 100.0f);
        // render container
        ourShader.activate();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        ourShader.setFloat("mixVal", mixVal);
        lamp.pointLight.render(ourShader);
        
        cube.render(ourShader);
        
        lampShader.activate();
        lampShader.setMat4("view", view);
        lampShader.setMat4("projection", projection);
        lamp.render(lampShader);
        
//        glDrawArrays(GL_TRIANGLES, 0, 36);
        screen.newFrame();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    cube.cleanup();
    lamp.cleanup();
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
    } else if(Keyboard::key(GLFW_KEY_UP)) {
        cameras[activeCamera].updateCameraDirection(0, 1.0f);
    } else if(Keyboard::key(GLFW_KEY_DOWN)) {
        cameras[activeCamera].updateCameraDirection(0, -1.0f);
    } else if(Keyboard::key(GLFW_KEY_LEFT)) {
        cameras[activeCamera].updateCameraDirection(1.0f, 0);
    } else if(Keyboard::key(GLFW_KEY_RIGHT)) {
        cameras[activeCamera].updateCameraDirection(-1.0f, 0);
    }
    
//    double dx = Mouse::getDX(), dy = Mouse::getDY();
//    if(dx != 0 && dy != 0) {
//        cameras[activeCamera].updateCameraDirection(dx/10, dy/10);
//    }

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
