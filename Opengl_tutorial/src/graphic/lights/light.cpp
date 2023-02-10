//
//  light.cpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/26.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include "light.hpp"

PointLight::PointLight() {}

PointLight::PointLight(glm::vec3 pos, float k1, float k2, float k3, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : position(pos), k1(k1), k2(k2), k3(k3), ambient(ambient), diffuse(diffuse), specular(specular) {}

void PointLight::render(Shader shader, int index) {
    std::string name = "pointLights[" + std::to_string(index) + "]";
    
    shader.setFloat3(name + ".position", position);
    
    shader.setFloat(name + ".k1", k1);
    shader.setFloat(name + ".k2", k2);
    shader.setFloat(name + ".k3", k3);
    
    shader.setFloat4(name + ".ambient", glm::vec4(ambient, 1.0f));
    shader.setFloat4(name + ".diffuse", glm::vec4(diffuse, 1.0f));
    shader.setFloat4(name + ".specular", glm::vec4(specular, 1.0f));
}

void SpotLight::render(Shader shader, int index) {
    std::string name = "spotLights[" + std::to_string(index) + "]";
    
    shader.setFloat3(name + ".position", position);
    shader.setFloat3(name + ".direction", direction);
    
    shader.setFloat(name + ".k1", k1);
    shader.setFloat(name + ".k2", k2);
    shader.setFloat(name + ".k3", k3);
    
    shader.setFloat(name + ".cutOff", cutOff);
    shader.setFloat(name + ".outerCutOff", outerCutOff);
    
    shader.setFloat4(name + ".ambient", glm::vec4(ambient, 1.0f));
    shader.setFloat4(name + ".diffuse", glm::vec4(diffuse, 1.0f));
    shader.setFloat4(name + ".specular", glm::vec4(specular, 1.0f));
}

DirLight::DirLight(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular) {}

void DirLight::render(Shader shader) {
    std::string name = "dirLight";

    shader.setFloat3(name + ".direction", direction);
    shader.setFloat4(name + ".ambient", glm::vec4(ambient, 1.0f));
    shader.setFloat4(name + ".diffuse", glm::vec4(diffuse, 1.0f));
    shader.setFloat4(name + ".specular", glm::vec4(specular, 1.0f));
    
}
