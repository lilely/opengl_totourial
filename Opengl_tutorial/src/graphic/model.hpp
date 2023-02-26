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

#include "../algorithm/bounds.hpp"
#include "material.hpp"
#include "../physics/rigidbody.hpp"
#include "mesh.hpp"
#include "./models/box.hpp"
#include "../physics/rigidbody.hpp"
#include "../scene.hpp"

typedef enum ModelSwitches : unsigned int {
    DYNAMIC = 0x01,
    CONST_INSTANCES = 0x02,
    NO_TEX = 0x04
} ModelSwitches;

// Forward declaration
class Scene;

class Model {
public:
    std::string id;
    
    RigidBody rb;
    
    glm::vec3 size;
    
    std::vector<Mesh> meshes;
    
    std::vector<BoundingRegion> boundingRegions;
    
    std::vector<RigidBody *> instances;
    
    unsigned int maxNoInstances;
    
    unsigned int currentNoInstances;
    
    // combination of switches above
    unsigned int switches;
    
    BoudingTypes boundType;
    
    Model(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f), bool hasTex = true, BoudingTypes boundType = BoudingTypes::AABB);
    
    Model(std::string id, BoudingTypes boundType, unsigned int maxNoInstances, unsigned int flag = 0);
    
    RigidBody *generateInstance(glm::vec3 pos, float mass, glm::vec3 size);
    
    void loadModel(std::string path);
    
    void initInstances();
    
    void removeInstance(unsigned int idx);
    
    void removeInstance(std::string instanceId);
    
    unsigned int getIdx(std::string id);
    
    virtual void init();
    
    virtual void render(Shader &shader, float dt, Scene *scene, bool setModel = true);
    
    void cleanup();
    
protected:
    
    bool hasTex;
    
    BufferObject posVBO;
    
    BufferObject sizeVBO;
    
    std::string directory;
    
    std::vector<Texture> textures_loaded;
    
    void processNode(aiNode *node, const aiScene* scene);
    
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    
    std::vector<Texture> loadTextures(aiMaterial* mat, aiTextureType type);
};

#endif /* model_hpp */
