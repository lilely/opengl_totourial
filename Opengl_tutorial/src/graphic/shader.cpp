//
//  Shader.c
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/18.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include "shader.hpp"

Shader::Shader(){};

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath) {
    generateProgram(vertexShaderPath, fragmentShaderPath);
}

void Shader::generateProgram(const char *vertexShaderPath, const char * fragmentShaderPath) {
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
        std::cout << filepath << ",Error with shader comp.:" << std::endl << infoLog << std::endl;
    }
    return shader;
}

void Shader::setMat4(const std::string &name, glm::mat4 val) {
    glUniformMatrix4fv(glGetUniformLocation(id,name.c_str()),1,GL_FALSE,glm::value_ptr(val));
}

void Shader::setMat3(const std::string &name, glm::mat3 val) {
    glUniformMatrix3fv(glGetUniformLocation(id,name.c_str()),1,GL_FALSE,glm::value_ptr(val));
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat4(const std::string& name, glm::vec4 val) {
    glUniform4f(glGetUniformLocation(id, name.c_str()), val.x, val.y, val.z, val.w);
}

void Shader::setFloat4(const std::string& name, aiColor4D val) {
    glUniform4f(glGetUniformLocation(id, name.c_str()), val.r, val.g, val.b, val.a);
}

void Shader::setFloat3(const std::string& name, glm::vec3 val) {
    setFloat3(name, val.x, val.y, val.z);
}

void Shader::setFloat3(const std::string& name, float v1, float v2, float v3) {
    glUniform3f(glGetUniformLocation(id, name.c_str()), v1, v2, v3);
}
