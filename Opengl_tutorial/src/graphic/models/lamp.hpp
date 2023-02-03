//
//  lamp.hpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/26.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef lamp_h
#define lamp_h

#include "cube.hpp"
#include "../lights/light.hpp"
#include "modelarray.hpp"

class Lamp : public Cube {
public:
    glm::vec3 lightColor;
    PointLight pointLight;
    
    Lamp(){};
    
    Lamp(glm::vec3 lightColor,
         glm::vec3 ambient,
         glm::vec3 diffuse,
         glm::vec3 specular,
         glm::vec3 pos,
         float k1,
         float k2,
         float k3,
         glm::vec3 size) : lightColor(lightColor),
        pointLight({pos, k1, k2, k3, ambient, diffuse, specular}),
        Cube(pos, size) {};

    void render(Shader shader, float dt) {
        shader.setFloat3("lightColor", lightColor);
        Cube::render(shader, dt);
    }
};

class LampArray : public ModelArray<Lamp> {
public:
    std::vector<PointLight> pointLights;
    
    void init() {
        model = Lamp(glm::vec3(1.0f),glm::vec3(1.0f),glm::vec3(1.0f),glm::vec3(1.0f),glm::vec3(0.0f), 1.0f, 0.07f, 0.005f, glm::vec3(0.25f));
        ModelArray::init();
    }
    
    void render(Shader shader, float dt) {
        for(auto &light : pointLights) {
            model.rb.pos = light.position;
            shader.activate();
            model.render(shader, dt);
        }
    }
    
};

#endif /* lamp_h */
