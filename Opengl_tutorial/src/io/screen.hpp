//
//  screen.hpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/24.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef screen_hpp
#define screen_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Screen {
public:
    static unsigned int SCR_WIDTH;
    static unsigned int SCR_HEIGHT;
    static void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
    
    Screen();
    
    bool init();
    
    void setParamters();
    
    void update();
    
    void newFrame();
    
    bool shouldClose();
    
    void setShouldClose(bool shouldClose);
    
private:
    GLFWwindow* window;
};

#endif /* screen_hpp */
