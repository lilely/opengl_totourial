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
#include "../memory/vertexmemory.hpp"

#define UPPER_BOUND 100

template<class T>
class ModelArray {
public:
    BufferObject posVBO;
    
    BufferObject sizeVBO;
    
    std::vector<RigidBody> instances;
    
    std::vector<glm::vec3> positions;
    
    std::vector<glm::vec3> sizes;
    
    void init() {
        model.init();
        setupVBO();
    }
    
    void setupVBO() {
        posVBO.generate();
        posVBO.bind();
        posVBO.setData<glm::vec3>(UPPER_BOUND, NULL, GL_DYNAMIC_DRAW);
        posVBO.clear();
        
        sizeVBO.generate();
        sizeVBO.bind();
        sizeVBO.setData<glm::vec3>(UPPER_BOUND, NULL, GL_DYNAMIC_DRAW);
        sizeVBO.clear();
        for(int i = 0;i < model.meshes.size();i++) {
            model.meshes[i].VAO.bind();
            // positions
            posVBO.bind();
            posVBO.setAttPointer<glm::vec3>(3, 3, GL_FLOAT, 1, 0, 1);
            // size
            sizeVBO.bind();
            sizeVBO.setAttPointer<glm::vec3>(4, 3, GL_FLOAT, 1, 0, 1);
            sizeVBO.clear();
            
            ArrayObject::clear();
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
            posVBO.bind();
            posVBO.updateData<glm::vec3>(0, size, &positions[0]);
            
            sizeVBO.bind();
            sizeVBO.updateData<glm::vec3>(0, size, &sizes[0]);
            
            sizeVBO.clear();
        }
        for(unsigned int i = 0;i < model.meshes.size();i++) {
            if (box != nullptr) {
                for(unsigned int j = 0;j < size;j++) {
                    box->addInstance(model.meshes[i].boundRange, positions[j], sizes[j]);
                }
            }
            model.meshes[i].VAO.bind();
            model.meshes[i].VAO.draw(GL_TRIANGLES, static_cast<int>(model.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, (GLsizei)size);
            ArrayObject::clear();
            // reset
            glActiveTexture(GL_TEXTURE0);
        }
    }
    
    void setSize(glm::vec3 size) {
        model.size = size;
    }
    
    void cleanup() {
        model.cleanup();
        posVBO.cleanup();
        sizeVBO.cleanup();
    }
    
protected:
    T model;
};

#endif /* modelarray_hpp */
