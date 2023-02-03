//
//  model.hpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/24.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef model_hpp
#define model_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include "material.hpp"
#include "../physics/rigidbody.hpp"
#include "mesh.hpp"

class Model {
public:
    RigidBody rb;
    
    glm::vec3 size;
    
    std::vector<Mesh> meshes;
    
    Model(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f), bool hasTex = true);
    
    void loadModel(std::string path);
    
    void init();
    
    void render(Shader &shader, float dt, bool setModel = true, bool doRender = true);
    
    void cleanup();
    
protected:
    
    bool hasTex;
    
    std::string directory;
    
    std::vector<Texture> textures_loaded;
    
    void processNode(aiNode *node, const aiScene* scene);
    
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    
    std::vector<Texture> loadTextures(aiMaterial* mat, aiTextureType type);
};

#endif /* model_hpp */
