//
//  scene.cpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/2/9.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include "scene.hpp"

#include "io/keyboard.hpp"
#include "io/mouse.hpp"
#include "io/joystick.hpp"
#include "algorithm/states.hpp"

unsigned int Scene::SCR_WIDTH = 800;
unsigned int Scene::SCR_HEIGHT = 600;

void Scene::frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    Scene::SCR_WIDTH = width;
    Scene::SCR_HEIGHT = height;
}


Scene::Scene(int glfwVersionMajor, int glfwVersionMinor, const char *title, unsigned int scrWidth, unsigned int scrHeight) : glfwVersionMajor(glfwVersionMajor), glfwVersionMinor(glfwVersionMinor), title(title), activeCamera(-1) {
    Scene::SCR_WIDTH = scrWidth;
    Scene::SCR_HEIGHT = scrHeight;
    
    setWindowColor(0.1f, 0.15f, 0.15f, 1.0f);
}

bool Scene::init() {
    glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfwVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfwVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // intialize window
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, NULL, NULL);
    if(window == NULL) {
        return false;
    }
    glfwMakeContextCurrent(window);
    
    // set GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std:: endl;
        glfwTerminate();
        return false;
    }
    
    // setup screen
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    
    /*
      callbacks
     */
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    
    glfwSetKeyCallback(window, Keyboard::keyCallback);
    
    glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
    
    glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
    
    glfwSetScrollCallback(window, Mouse::mouseWheelCallback);
    
    /*
      set rending parameters
     */
    glEnable(GL_DEPTH_TEST); // dosen't show verices not visible
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    return true;
}

/*
    main loop methods
 */

void Scene::processInput(float delta)
{
    if(Keyboard::keyWentUp(GLFW_KEY_ESCAPE)) {
        setShouldClose(true);
    }
    if(Keyboard::key(GLFW_KEY_W)) {
        cameras[activeCamera]->updateCameraPos(CameraDirection::FORWARD, delta);
    }
    if(Keyboard::key(GLFW_KEY_S)) {
        cameras[activeCamera]->updateCameraPos(CameraDirection::BACKWARD, delta);
    }
    if(Keyboard::key(GLFW_KEY_A)) {
        cameras[activeCamera]->updateCameraPos(CameraDirection::LEFT, delta);
    }
    if(Keyboard::key(GLFW_KEY_D)) {
        cameras[activeCamera]->updateCameraPos(CameraDirection::RIGHT, delta);
    }
    if(Keyboard::key(GLFW_KEY_E)) {
        cameras[activeCamera]->updateCameraPos(CameraDirection::UP, delta);
    }
    if(Keyboard::key(GLFW_KEY_X)) {
        cameras[activeCamera]->updateCameraPos(CameraDirection::DOWN, delta);
    }
    if(Keyboard::keyWentDown(GLFW_KEY_TAB)) {
        activeCamera += activeCamera == 0 ? 1 : -1;
    }
    if(Keyboard::key(GLFW_KEY_UP)) {
        cameras[activeCamera]->updateCameraDirection(0, 1.0f);
    }
    if(Keyboard::key(GLFW_KEY_DOWN)) {
        cameras[activeCamera]->updateCameraDirection(0, -1.0f);
    }
    if(Keyboard::key(GLFW_KEY_LEFT)) {
        cameras[activeCamera]->updateCameraDirection(-1.0f, 0);
    }
    if(Keyboard::key(GLFW_KEY_RIGHT)) {
        cameras[activeCamera]->updateCameraDirection(1.0f, 0);
    }
//    if(Keyboard::keyWentDown(GLFW_KEY_L)) {
//        should = !needSpotLight;
//    }
    
//    if(Keyboard::keyWentDown(GLFW_KEY_F)) {
//        addSphere();
//    }
    
    
//    double dx = Mouse::getDX(), dy = Mouse::getDY();
//    if(dx != 0 && dy != 0) {
//        cameras[activeCamera].updateCameraDirection(dx/10, dy/10);
//    }

//    double scrollDy = Mouse::getScrollDY();
//    if(scrollDy != 0) {
//        std::cout << scrollDy << std::endl;
//        camera.updateCameraZoom(scrollDy);
//    }
    
//    mainJ.update();

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

void Scene::update() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::newFrame() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Scene::render(Shader shader, bool applyLighting) {
    
}

/* clean up methods */
void Scene::cleanup() {
    
}

/* accessors */
Camera* Scene::getActiveCamera() {
    return cameras[activeCamera];
}

bool Scene::shouldClose() {
    return glfwWindowShouldClose(window);
}

/* modifiers */

void Scene::setShouldClose(bool shouldClose) {
    glfwSetWindowShouldClose(window, shouldClose);
}

void Scene::setWindowColor(float r, float g, float b, float a) {
    setWindowColor(r, g, b, a);
}

