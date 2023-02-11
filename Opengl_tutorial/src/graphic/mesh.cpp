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

Mesh::Mesh(BoundingRegion boundRange, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) : boundRange(boundRange), vertices(vertices), indices(indices), textures(textures), hasTexture(true) {
    setup();
}

Mesh::Mesh(BoundingRegion boundRange, std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diffuse, aiColor4D specular) : boundRange(boundRange), vertices(vertices), indices(indices), material_diffuse(diffuse), material_specular(specular) , hasTexture(false) {
    setup();
}

void Mesh::render(Shader &shader,Box *box, glm::vec3 pos, glm::vec3 size, bool doRender) {
    if(!hasTexture) {
        shader.setFloat4("material.diffuse", material_diffuse);
        shader.setFloat4("material.specular", material_specular);
        shader.setInt("hasTexture", 0);
    } else {
        unsigned int diffuseTextureIndex = 0;
        unsigned int specularTexutreIndex = 0;
        shader.setInt("hasTexture", 1);
        for(unsigned int i = 0;i < textures.size();i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string name;
            switch (textures[i].type) {
                case aiTextureType_DIFFUSE:
                    name = "diffuse" + std::to_string(diffuseTextureIndex++);
                    break;
                case aiTextureType_SPECULAR:
                    name = "specular" + std::to_string(specularTexutreIndex++);
                    break;
            }
            shader.setInt(name, i);
            textures[i].bind();
        }
    }
    
    if(doRender) {
        if(box) {
            box->addInstance(boundRange, pos, size);
        }
        VAO.bind();
        VAO.draw(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, 0);
        ArrayObject::clear();

        glActiveTexture(GL_TEXTURE0);
    }
}

void Mesh::cleanup() {
    VAO.cleanup();
}

void Mesh::setup() {
    VAO.generate();
    VAO.bind();
    
    VAO["EBO"] = BufferObject(GL_ELEMENT_ARRAY_BUFFER);
    VAO["EBO"].generate();
    VAO["EBO"].bind();
    VAO["EBO"].setData(static_cast<GLuint>(indices.size()), &indices[0], GL_STATIC_DRAW);
    
    VAO["VBO"] = BufferObject(GL_ARRAY_BUFFER);
    VAO["VBO"].generate();
    VAO["VBO"].bind();
    VAO["VBO"].setData<Vertex>(static_cast<GLuint>(vertices.size()), &vertices[0], GL_STATIC_DRAW);
    // position attribute
    VAO["VBO"].setAttPointer<float>(0, 3, GL_FLOAT, 8, 0);
    // normal
    VAO["VBO"].setAttPointer<float>(1, 3, GL_FLOAT, 8, offsetof(Vertex, normal));
    // color attribute
    VAO["VBO"].setAttPointer<float>(2, 2, GL_FLOAT, 8, offsetof(Vertex, texCoord));
    VAO["VBO"].clear();

    VAO.clear();    
}
