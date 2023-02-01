//
//  modelarray.hpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/2/1.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef modelarray_hpp
#define modelarray_hpp

#include "../model.hpp"
#include <vector>

template<class T>
class ModelArray {
public:
    std::vector<RigidBody> instances;
    
    void init() {
        model.init();
    }
    
    void render(Shader shader, float dt) {
        for(RigidBody &body : instances) {
            body.update(dt);
            model.rb.pos = body.pos;
            model.render(shader, dt);
        }
    }
    
    void setSize(glm::vec3 size) {
        model.size = size;
    }
    
    void cleanup() {
        model.cleanup();
    }
    
protected:
    T model;
};

#endif /* modelarray_hpp */
