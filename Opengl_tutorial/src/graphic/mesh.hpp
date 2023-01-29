//
//  mesh.hpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/24.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef mesh_hpp
#define mesh_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "texture.hpp"

typedef struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoord;
    
    static std::vector<struct Vertex> genList(float* vertices, int noVertices);
} Vertex;

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO;
    
    std::vector<Texture> textures;
    
    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures = {});
    
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diffuse, aiColor4D specular);
    
    void render(Shader &shader);
    
    void cleanup();
    
private:
    
    unsigned int VBO, EBO;
    
    bool hasTexture;
    
    aiColor4D material_diffuse;
    
    aiColor4D material_specular;
    
    void setup();
};

#endif /* mesh_hpp */
