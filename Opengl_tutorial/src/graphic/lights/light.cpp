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
    shader.setFloat3(name + ".ambient", ambient);
    shader.setFloat3(name + ".diffuse", diffuse);
    shader.setFloat3(name + ".specular", specular);
}
