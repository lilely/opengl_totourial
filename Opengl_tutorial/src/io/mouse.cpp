//
//  mouse.cpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/20.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include "mouse.hpp"

double Mouse::x = 0;
double Mouse::y = 0;

double Mouse::lastX = 0;
double Mouse::lastY = 0;

double Mouse::dx = 0;
double Mouse::dy = 0;

double Mouse::scrollDx = 0;
double Mouse::scrollDy = 0;

bool Mouse::firstMouse = false;

bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = {0};
bool Mouse::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = {0};

void Mouse::cursorPosCallback(GLFWwindow *window, double _x, double _y){
    x = _x;
    y = _y;
    if(firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }
    dx = x - lastX;
    dy = lastY - y;
    lastX = x;
    lastY = y;
}

void Mouse::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
    if(action != GLFW_RELEASE) {
        if(!buttons[button]) {
            buttons[button] = true;
        }
    } else {
        buttons[button] = false;
    }
    buttonsChanged[button] = action != GLFW_REPEAT;
}

void Mouse::mouseWheelCallback(GLFWwindow *window, double dx, double dy){
    scrollDx = dx;
    scrollDy = dy;
}

double Mouse::getMouseX() {
    return x;
}

double Mouse::getMouseY() {
    return y;
}

double Mouse::getDX() {
    return dx;
}
double Mouse::getDY() {
    return dy;
}

double Mouse::getScrollDX() {
    return scrollDx;
}
double Mouse::getScrollDY() {
    return scrollDy;
}

bool Mouse::button(int button) {
    return buttons[button];
}
bool Mouse::buttonChanged(int button) {
    bool ret = buttonsChanged[button];
    buttonsChanged[button] = false;
    return ret;
}
bool Mouse::buttonWentUp(int button) {
    return buttons[button] && buttonChanged(button);
}
bool Mouse::buttonWentDown(int button) {
    return buttons[button] && buttonChanged(button);
}

