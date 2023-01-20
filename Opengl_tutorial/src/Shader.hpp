//
//  Shader.h
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/18.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef Shader_h
#define Shader_h

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    unsigned int id;
    
    Shader(const char *vertexShaerPath, const char * fragmentShaderPath);
    
    std::string loadShaderSrc(const char * filePath);
    
    void activate();
    
    GLuint compileShader(const char* filepath, GLenum type);
    
    void setMat4(const std::string &name, glm::mat4 val);
    
    void setInt(const std::string& name, int value);
    
    void setFloat(const std::string& name, float value);
};

#endif /* Shader_h */
