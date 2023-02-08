//
//  box.h
//  Opengl_tutorial
//
//  Created by 星金 on 2023/2/5.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef box_h
#define box_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "../../algorithm/bounds.hpp"
#include "../memory/vertexmemory.hpp"

#define BOX_UPPER_BOUND 100

class Box {
public:
    
    std::vector<glm::vec3> offsetVecs;
    std::vector<glm::vec3> sizeVecs;
    
    void init() {
        vertices = {
            // position             x   y   z   i
             0.5f,  0.5f,  0.5f, // +   +   +   0
            -0.5f,  0.5f,  0.5f, // -   +   +   1
            -0.5f, -0.5f,  0.5f, // -   -   +   2
             0.5f, -0.5f,  0.5f, // +   -   +   3
             0.5f,  0.5f, -0.5f, // +   +   -   4
            -0.5f,  0.5f, -0.5f, // -   +   -   5
            -0.5f, -0.5f, -0.5f, // -   -   -   6
             0.5f, -0.5f, -0.5f  // +   -   -   7
        };
         
        indices = { // 12 lines
            // front face (+ve z)
            0, 1,
            1, 2,
            2, 3,
            3, 0,
            // back face (-ve z)
            4, 5,
            5, 6,
            6, 7,
            7, 4,
            // right face (+ve x)
            0, 4,
            3, 7,
            // left face (-ve x)
            1, 5,
            2, 6
        };
        VAO.generate();
        VAO.bind();
        
        VBO.generate();
        VBO.bind();
        VBO.setData<GLfloat>(static_cast<GLuint>(vertices.size() * 3), &vertices[0], GL_STATIC_DRAW);
        VBO.clear();
        
        offsetVBO.generate();
        offsetVBO.bind();
        offsetVBO.setData<glm::vec3>(BOX_UPPER_BOUND, NULL, GL_DYNAMIC_DRAW);
        offsetVBO.clear();

        
        sizeVBO.generate();
        sizeVBO.bind();
        sizeVBO.setData<glm::vec3>(BOX_UPPER_BOUND, NULL, GL_DYNAMIC_DRAW);
        sizeVBO.clear();

        EBO = BufferObject(GL_ELEMENT_ARRAY_BUFFER);
        EBO.generate();
        EBO.bind();
        EBO.setData<GLuint>(static_cast<GLuint>(indices.size()), &indices[0], GL_STATIC_DRAW);
        
        // position attribute
        VBO.bind();
        VBO.setAttPointer<float>(0, 3, GL_FLOAT, 3, 0);
        VBO.clear();
        
        // position
        offsetVBO.bind();
        offsetVBO.setAttPointer<glm::vec3>(1, 3, GL_FLOAT, 1, 0, 1);
        offsetVBO.clear();

        // size
        sizeVBO.bind();
        sizeVBO.setAttPointer<glm::vec3>(2, 3, GL_FLOAT, 1, 0, 1);
        sizeVBO.clear();

        ArrayObject::clear();
    }
    
    void render(Shader shader, float dt) {
        shader.setMat4("model", glm::mat4(1.0f));
        size_t size = std::min((int)offsetVecs.size(), BOX_UPPER_BOUND);
        if(size != 0) {
            offsetVBO.bind();
            offsetVBO.updateData<glm::vec3>(0, (unsigned int)size, &offsetVecs[0]);

            sizeVBO.bind();
            sizeVBO.updateData<glm::vec3>(0, (unsigned int)size, &sizeVecs[0]);
            
            sizeVBO.clear();
        }
        VAO.bind();
        VAO.draw(GL_LINES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0, (GLsizei)size);
        ArrayObject::clear();
    }
    
    void addInstance(BoudingRegion &rb, glm::vec3 pos, glm::vec3 size) {
        offsetVecs.push_back(rb.caculateCenter() * size + pos);
        sizeVecs.push_back(rb.caculateDimensions() * size);
    }
    
    void cleanup() {
        VAO.cleanup();
        VBO.cleanup();
        EBO.cleanup();
        offsetVBO.cleanup();
        sizeVBO.cleanup();
    }
    
private:
    ArrayObject VAO;
    BufferObject VBO, offsetVBO, sizeVBO;
    BufferObject EBO;
    
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};


#endif /* box_h */
