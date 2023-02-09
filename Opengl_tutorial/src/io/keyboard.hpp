//
//  keyboard.hpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/20.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef keyboard_hpp
#define keyboard_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Keyboard {
public:
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static bool key(int key);
    static bool keyChanged(int key);
    static bool keyWentUp(int key);
    static bool keyWentDown(int key);
private:
    static bool keys[];
    static bool keysChanged[];
};


#endif /* keyboard_hpp */
