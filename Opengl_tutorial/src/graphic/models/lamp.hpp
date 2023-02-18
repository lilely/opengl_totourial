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
#include "box.hpp"
#include <memory>

class Lamp : public Cube {
public:
    glm::vec3 lightColor;
    
    Lamp(unsigned int maxNoInstances, glm::vec3 lightColor = glm::vec3(1.0f)) : Cube(maxNoInstances){
        id = "Lamp";
        this->lightColor = lightColor;
    };

    void render(Shader &shader, float dt, Scene *scene, bool setModel = true) {
        shader.setFloat3("lightColor", lightColor);
        Cube::render(shader, dt, scene, setModel);
    }
};

#endif /* lamp_h */
