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

#define UPPER_BOUND 100

template<class T>
class ModelArray {
public:
    unsigned int posVBO;
    
    unsigned int sizeVBO;
    
    std::vector<RigidBody> instances;
    
    std::vector<glm::vec3> positions;
    
    std::vector<glm::vec3> sizes;
    
    void init() {
        model.init();
    }
    
    void setupVBO() {
        unsigned int posVBO;
        glGenBuffers(1, &posVBO);
        glBindBuffer(GL_ARRAY_BUFFER, posVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * UPPER_BOUND, &positions[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1, &sizeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * UPPER_BOUND, &sizes[0], GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        for(int i = 0;i < model.meshes.size();i++) {
            glBindVertexArray(model.meshes[i].VAO);
            // positions
            glBindBuffer(GL_ARRAY_BUFFER, posVBO);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
            glEnableVertexAttribArray(3);
            // sizes
            glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
            glEnableVertexAttribArray(4);
            
            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            
            glBindVertexArray(0);
        }
        
    }
    
    void render(Shader shader, float dt, bool setLists = true) {
        if(setLists) {
            positions.clear();
            sizes.clear();
            for(RigidBody &rb : instances) {
                rb.update(dt);
                positions.push_back(rb.pos);
                sizes.push_back(model.size);
//                model.rb.pos = rb.pos;
//                model.render(shader, dt);
            }
            shader.setMat4("model", glm::mat4(1.0f));
            model.render(shader, dt, false, false);
            if(positions.size() != 0) {
                glBindBuffer(GL_ARRAY_BUFFER, posVBO);
            }
        } else {
            for(RigidBody &rb : instances) {
                rb.update(dt);
                model.rb.pos = rb.pos;
                model.render(shader, dt);
            }
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
