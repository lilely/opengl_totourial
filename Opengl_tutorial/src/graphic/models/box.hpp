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
        
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1, &offsetVBO);
        glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * BOX_UPPER_BOUND, NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1, &sizeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * BOX_UPPER_BOUND, NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        
        // position attribute
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // positions
        glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
        glEnableVertexAttribArray(1);
        // sizes
        glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
        glEnableVertexAttribArray(2);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glVertexAttribDivisor(1, 1);
        glVertexAttribDivisor(2, 1);
        
        glBindVertexArray(0);
    }
    
    void render(Shader shader, float dt) {
        shader.setMat4("model", glm::mat4(1.0f));
        size_t size = std::min((int)offsetVecs.size(), BOX_UPPER_BOUND);
        if(size != 0) {
            glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * size, &offsetVecs[0]);

            glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * size, &sizeVecs[0]);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_LINES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0, (GLsizei)size);
        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
    
    void cleanup() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &offsetVBO);
        glDeleteBuffers(1, &sizeVBO);
    }
    
private:
    unsigned int VAO, VBO, EBO, offsetVBO, sizeVBO;
    
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};


#endif /* box_h */
