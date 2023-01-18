//
//  main.cpp
//  Opengl_tutorial
//
//  Created by 金星 on 2023/1/13.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <stb/stb_image.h>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include "Shader.hpp"

int main(int argc, char **argv){
    using std::cout;    using std::endl;

    //glm test
//    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
//    glm::mat4 trans = glm::mat4(1.0f);
    
//    trans = glm::translate(trans, glm::vec3(2.0f, 1.0f, 1.0f));
    
//    trans = glm::rotate(trans,glm::radians(180.0f),glm::vec3(0.0f, 0.0f, 1.0f));
    
//    trans = glm::scale(trans,glm::vec3(0.5, 0.5, 0.5));
    
//    vec = trans * vec;
//    std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
    
    glfwInit();
//    主版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    次版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    告诉glfw 我们使用的是core_profile 核心模块
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    向前兼容
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//    创建一个GLFW 窗口   宽 高  窗口名字  后边两个暂时不用管
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
//    通知GLFW将我们窗口的上下文设置为当前线程的主上下文了
    glfwMakeContextCurrent(window);
    
//    GLAD 是用来管理openGL 的函数指针的。所以在调用任何openGL函数之前我们都需要初始化GLAD。
//    我们给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。GLFW给我们的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数。
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //    创建渲染的视口: 我们必须要告诉OpenGl 需要渲染的尺寸大小，即为视口 viewport(),这样openGL 才能知道根据窗口大小显示数据和坐标。
//    glViewport 前两个参数控制视口左下角位置，后两个参数控制视口的宽和高
//    openGL 幕后使用的是glViewport 定义的 位置和宽高进行2D转换
    glViewport(0, 0, 800, 600);
//    窗口调整的时候 视口应该也被调整  对窗口注册一个回调函数每次窗口大小被调整的时候会被调用
    void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    

//   通过 glfwSetFramebufferSizeCallback glfw 函数 当窗口frame 变化时会调用。
//    对于视网膜屏 Retain 屏   宽度和高度明显比原输入值更高一点。
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    void processInput(GLFWwindow *window);
    
    /* shaders
     
     */
    Shader shader("/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/vertex_core.glsl","/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/fragment_core.glsl");
    Shader shader2("/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/vertex_core.glsl","/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/fragment_core_2.glsl");
    // shader program 0
//    glBindAttribLocation(shaderProgram[0], 0, "aPos");

    float vertics[] = {
        // vertics                   // colors
        0.5f, 0.5f, 0.0f,          0.3f, 0.7f, 0.5f,
        -0.5f, 0.5f, 0.0f,         0.5f, 0.2f, 0.75f,
        -0.5f, -0.5f, 0.0f,        0.6f, 0.5f, 0.2f,
        0.5f, -0.5f, 0.0f,         1.0f, 0.2f, 1.0f,
    };
    
    unsigned int indices[] = {
        0,1,2,
        2,3,0,
    };
    
    // VAO, VBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    
    // bind VAO
    glBindVertexArray(VAO);
    
    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertics),vertics,GL_STATIC_DRAW);
    
    // set vertex attribute pointer
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(void *)0);
    glEnableVertexAttribArray(0);
    
    // set color attribute pointer
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(void *)(3 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    
    // set up EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f,0.0f,1.0f));
    shader.setMat4("transform", trans);
    shader.activate();
    
    
    glm::mat4 trans2 = glm::mat4(1.0f);
    trans2 = glm::scale(trans,glm::vec3(1.5f));
    trans2 = glm::rotate(trans2, glm::radians(45.f),glm::vec3(0.0f,0.0f,1.0f));
    shader2.setMat4("transform", trans2);
    shader2.activate();
    
    
//   为了防止 渲染的图像一出现就退出 我们使用while 循环 。我们可以称之为Render Loop
//    glfwWindowShouldClose 每次循环开始前检查一次GLFW 是否被要求退出 是true 的话渲染便结束了。
    while(!glfwWindowShouldClose(window))
    {
        //输出控制
        processInput(window);
//        glfwSwapBuffers 会交换颜色缓冲（他是存储着GLFW 窗口每一个像素色值的大缓冲），将会作为输出显示在屏幕上
//        当程序退出的时候 使用一个自定义的颜色清空屏幕  在每个新的渲染迭代可是的时候我们总希望清屏否则总是看到上次渲染的结果。
//        我们可以使用glClear   GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT。 我们清空颜色 。
        glClearColor(0.5f, 0.1f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // draw shapes
        glBindVertexArray(VAO);
        
//        trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 5.f), glm::vec3(0.0f,0.0f,1.0f));
        trans = glm::rotate(trans, glm::radians(0.1f), glm::vec3(0.0f,0.0f,1.0f));
        shader.setMat4("transform", trans);
        shader.activate();
//        glDrawArrays(GL_TRIANGLES,0,6);
        glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
        
        trans2 = glm::rotate(trans2, glm::radians(-0.1f), glm::vec3(0.0f,0.0f,1.0f));
        shader2.setMat4("transform", trans2);
        shader2.activate();
        glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,(void *)(3*sizeof(GLuint)));
        
//        glUseProgram(shaderProgram[1]);
//        glDrawArrays(GL_TRIANGLES,0,6);
//        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void *)(3*sizeof(unsigned int)));
        
        glfwSwapBuffers(window);
//        glfwPollEvents 检查函数有没有触发什么事件 键盘输入 鼠标移动 并调用对应函数
        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();

    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
