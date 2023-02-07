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
#include <algorithm>
#include <glm/glm.hpp>
#include "box.hpp"

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
        setupVBO();
    }
    
    void setupVBO() {
        glGenBuffers(1, &posVBO);
        glBindBuffer(GL_ARRAY_BUFFER, posVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * UPPER_BOUND, NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1, &sizeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * UPPER_BOUND, NULL, GL_DYNAMIC_DRAW);
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
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            
            glBindVertexArray(0);
        }
        
    }
    
    void render(Shader shader, float dt, Box *box = nullptr, bool setLists = true) {
        if(setLists) {
            positions.clear();
            sizes.clear();
            for(RigidBody &rb : instances) {
                rb.update(dt);
                positions.push_back(rb.pos);
//                std::cout << "updated position:" << std::endl;
//                std::cout << rb.pos.x << " " << rb.pos.y << " " << rb.pos.z << std::endl;
                sizes.push_back(model.size);
            }
        } else {
//            std::cout << "updated size:" << std::endl;
//            std::cout << sizes[0].x << " " << sizes[0].y << " " << sizes[0].z << std::endl;
        }
        shader.setMat4("model", glm::mat4(1.0f));
        model.render(shader, dt, false, false);
        size_t size = std::min((int)positions.size(), UPPER_BOUND);
        if(positions.size() != 0) {
            glBindBuffer(GL_ARRAY_BUFFER, posVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * size, &positions[0]);

            glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * size, &sizes[0]);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        for(unsigned int i = 0;i < model.meshes.size();i++) {
            if (box != nullptr) {
                for(unsigned int j = 0;j < size;j++) {
                    box->addInstance(model.meshes[i].boundRange, positions[j], sizes[j]);
                }
            }
            glBindVertexArray(model.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<int>(model.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, (GLsizei)size);
            //    glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            // reset
            glActiveTexture(GL_TEXTURE0);
        }
    }
    
    void setSize(glm::vec3 size) {
        model.size = size;
    }
    
    void cleanup() {
        model.cleanup();
        glDeleteBuffers(1, &posVBO);
        glDeleteBuffers(1, &sizeVBO);
    }
    
protected:
    T model;
};

#endif /* modelarray_hpp */
