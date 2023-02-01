//
//  sphere.h
//  Opengl_tutorial
//
//  Created by 星金 on 2023/2/1.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef sphere_h
#define sphere_h

#include "../model.hpp"

class Sphere : public Model {
public:
    Sphere(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f)) : Model(pos, size, true) {}
    
    void init() {
        Model::loadModel("/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/models/sphere/scene.gltf");
    }
};


#endif /* sphere_h */