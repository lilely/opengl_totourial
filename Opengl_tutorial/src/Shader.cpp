//
//  Shader.c
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/18.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include "Shader.hpp"

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath) {
    int success;
    char infoLog[512];
    
    GLuint vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
    GLuint fragShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);
    
    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragShader);
    glLinkProgram(id);
    
    // catch error
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(id,512,NULL,infoLog);
        std::cout << "Link error :" << std::endl << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
}

std::string Shader::loadShaderSrc(const char * filePath) {
    std::ifstream file;
    std::stringstream buf;
    
    std::string ret = "";
    file.open(filePath);
    if(file.is_open()){
        buf << file.rdbuf();
        ret = buf.str();
    } else {
        std::cout << "Could not open " << filePath << std::endl;
    }
    file.close();
    
    return ret;
}

void Shader::activate() {
    glUseProgram(id);
}

GLuint Shader::compileShader(const char *filepath, GLenum type) {
    GLuint shader;
    shader = glCreateShader(type);
    std::string shaderSrc = loadShaderSrc(filepath);
    const GLchar *shaderStr = shaderSrc.c_str();
    glShaderSource(shader, 1, &shaderStr, NULL);
    glCompileShader(shader);
    GLint success = 0;
    char infoLog[512];
    // catch error
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader,512,NULL,infoLog);
        std::cout << "Error with shader comp.:" << std::endl << infoLog << std::endl;
    }
    return shader;
}

void Shader::setMat4(const std::string &name, glm::mat4 val) {
    glUniformMatrix4fv(glGetUniformLocation(id,name.c_str()),1,GL_FALSE,glm::value_ptr(val));
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
