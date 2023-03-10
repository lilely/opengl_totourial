//
//  joystick.cpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/20.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//


#include "joystick.hpp"

Joystick::Joystick(int i) {
    id = getId(i);
    update();
}

void Joystick::update() {
    present = glfwJoystickPresent(id);
    if(present) {
        name = glfwGetJoystickName(id);
        axes = glfwGetJoystickAxes(id, &axesCount);
        buttons = glfwGetJoystickButtons(id, &buttonCount);
    }
}

float Joystick::axesState(int axis) {
    if(present) {
        return axes[axis];
    }
    
    return -1;
}

unsigned char Joystick::buttonState(int button) {
    if(present) {
        return buttons[button];
    }
    return GLFW_RELEASE;
}

int Joystick::getAxesCount() {
    return axesCount;
}

int Joystick::getButtonCount() {
    return buttonCount;
}

bool Joystick::isPresent() {
    return present;
}

const char* Joystick::getName() {
    return name;
}

int Joystick::getId(int i) {
    return GLFW_JOYSTICK_1 + i;
}
