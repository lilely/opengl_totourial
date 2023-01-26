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

class Lamp : public Cube {
public:
    glm::vec3 lightColor;
    PointLight pointLight;
    
    Lamp(glm::vec3 lightColor,
         glm::vec3 ambient,
         glm::vec3 diffuse,
         glm::vec3 specular,
         glm::vec3 pos,
         glm::vec3 size) : lightColor(lightColor),
        pointLight({pos, ambient, diffuse, specular}),
        Cube(Material::white_plastic, pos, size) {};

    void render(Shader shader) {
        shader.setFloat3("lightColor", lightColor);
//        pointLight.render(shader);
        
        Cube::render(shader);
    }
};


#endif /* lamp_h */
