//
//  screen.cpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/24.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include "screen.hpp"

#include "keyboard.hpp"
#include "mouse.hpp"

unsigned int Screen::SCR_WIDTH = 800;

unsigned int Screen::SCR_HEIGHT = 600;

void Screen::frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    Screen::SCR_WIDTH = width;
    Screen::SCR_HEIGHT = height;
}

Screen::Screen() : window(nullptr) {};

bool Screen::init() {
    window = glfwCreateWindow(Screen::SCR_WIDTH, Screen::SCR_HEIGHT, "opengl tutorial", nullptr, nullptr);
    if(!window) {
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    
    return true;
}

void Screen::setParamters() {
    // key pressed
    glfwSetKeyCallback(window, Keyboard::keyCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
    glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
    glfwSetScrollCallback(window, Mouse::mouseWheelCallback);
    glEnable(GL_DEPTH_TEST);
}

void Screen::update() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Screen::newFrame() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Screen::shouldClose() {
    return glfwWindowShouldClose(window);
}

void Screen::setShouldClose(bool shouldClose) {
    glfwSetWindowShouldClose(window, shouldClose);
}

