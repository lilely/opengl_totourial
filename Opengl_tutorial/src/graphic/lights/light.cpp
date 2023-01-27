//
//  light.cpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/26.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include "light.hpp"

void PointLight::render(Shader shader) {
    std::string name = "pointLight";
    
    shader.setFloat3(name + ".position", position);
    
    shader.setFloat(name + ".k1", k1);
    shader.setFloat(name + ".k2", k2);
    shader.setFloat(name + ".k3", k3);
    
    shader.setFloat3(name + ".ambient", ambient);
    shader.setFloat3(name + ".diffuse", diffuse);
    shader.setFloat3(name + ".specular", specular);
}

void DirLight::render(Shader shader) {
    std::string name = "dirLight";
    
    shader.setFloat3(name + ".direction", direction);
    shader.setFloat3(name + ".ambient", ambient);
    shader.setFloat3(name + ".diffuse", diffuse);
    shader.setFloat3(name + ".specular", specular);
}

void SpotLight::render(Shader shader) {
    std::string name = "spotLight";
    
    shader.setFloat3(name + ".position", position);
    shader.setFloat3(name + ".direction", direction);
    
    shader.setFloat(name + ".k1", k1);
    shader.setFloat(name + ".k2", k2);
    shader.setFloat(name + ".k3", k3);
    
    shader.setFloat(name + ".cutOff", cutOff);
    shader.setFloat(name + ".outerCutOff", outerCutOff);
    
    shader.setFloat3(name + ".ambient", ambient);
    shader.setFloat3(name + ".diffuse", diffuse);
    shader.setFloat3(name + ".specular", specular);
    
}

