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
#include <glm/glm.hpp>
#include "modelarray.hpp"
#include "../../algorithm/bounds.hpp"

class Sphere : public Model {
public:
    Sphere(unsigned int maxNoInstances) : Model("Sphere", BoudingTypes::AABB, maxNoInstances, DYNAMIC | NO_TEX) {}
    
    void init() {
        Model::loadModel("/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/models/sphere/scene.gltf");
        
    }
};

#endif /* sphere_h */
