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

#include <assimp/scene.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    unsigned int id;
    
    Shader();
    
    Shader(const char *vertexShaerPath, const char * fragmentShaderPath);
    
    void generateProgram(const char *vertexShaderPath, const char * fragmentShaderPath);
    
    std::string loadShaderSrc(const char * filePath);
    
    void activate();
    
    GLuint compileShader(const char* filepath, GLenum type);
    
    void setMat4(const std::string &name, glm::mat4 val);
    
    void setMat3(const std::string &name, glm::mat3 val);
    
    void setInt(const std::string& name, int value);
    
    void setFloat(const std::string& name, float value);
    
    void setFloat3(const std::string& name, glm::vec3 val);
    
    void setFloat3(const std::string& name, float v1, float v2, float v3);
    
    void setFloat4(const std::string& name, glm::vec4 val);
    
    void setFloat4(const std::string& name, aiColor4D val);
};

#endif /* Shader_h */
