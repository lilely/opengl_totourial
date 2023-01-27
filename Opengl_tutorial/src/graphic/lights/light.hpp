//
//  light.hpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/26.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef light_hpp
#define light_hpp

#include <glm/glm.hpp>
#include "../shader.hpp"

struct PointLight {
    glm::vec3 position;
    
    float k1;
    float k2;
    float k3;
    
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    void render(Shader shader, int index);
};

typedef struct SpotLight {
    glm::vec3 position;
    glm::vec3 direction;
    
    float k1;
    float k2;
    float k3;
    
    float cutOff;
    float outerCutOff;
    
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    void render(Shader shader, int index);
} SpotLight;

typedef struct DirLight {
    glm::vec3 direction;
    
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    void render(Shader shader);
} DirLight;

#endif /* light_hpp */
