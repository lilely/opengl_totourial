//
//  mesh.cpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/24.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include "mesh.hpp"

std::vector<struct Vertex> Vertex::genList(float* vertices, int noVertices) {
    std::vector<Vertex> ret(noVertices);
    int stride = sizeof(Vertex) / sizeof(float);
    for(int i = 0;i < noVertices;i++) {
        ret[i].pos = glm::vec3(
                               vertices[stride * i],
                               vertices[stride * i + 1],
                               vertices[stride * i + 2]);
        ret[i].normal = glm::vec3(
                                  vertices[stride * i + 3],
                                  vertices[stride * i + 4],
                                  vertices[stride * i + 5]);
        ret[i].texCoord = glm::vec2(
                                     vertices[stride * i + 6],
                                     vertices[stride * i + 7]);
    }
    
    return ret;
}

Mesh::Mesh() {}
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) : vertices(vertices), indices(indices), textures(textures) {
    setup();
}

void Mesh::render(Shader &shader) {
    for(int i = 0;i < textures.size();i++) {
        glActiveTexture(GL_TEXTURE0 + textures[i].id);
        shader.setInt(textures[i].name, textures[i].id);
        textures[i].bind();
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0);
//    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);
    // color attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(offsetof(Vertex, texCoord)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    glBindVertexArray(0);
}
