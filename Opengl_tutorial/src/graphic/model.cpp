//
//  model.cpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/24.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include "model.hpp"
#include "../physics/enviroment.hpp"
#include "../algorithm/states.hpp"

#define UPPER_BOUND 100

Model::Model(glm::vec3 pos, glm::vec3 size, bool hasTex, BoudingTypes boundType) : boundType(boundType), size(size), hasTex(hasTex) {
    currentNoInstances = 0;
//    rb.pos = pos;
}

Model::Model(std::string id, BoudingTypes boundType, unsigned int maxNoInstances, unsigned int flag) : id(id) , boundType(boundType) , maxNoInstances(maxNoInstances), switches(flag) {
    currentNoInstances = 0;
}

RigidBody *Model::generateInstance(glm::vec3 pos, float mass, glm::vec3 size) {
    if(currentNoInstances >= maxNoInstances) {
        // all slots filled
        return nullptr;
    }
    instances.push_back(new RigidBody(id, size, mass, pos));
    return instances[currentNoInstances++];
}

unsigned int Model::getIdx(std::string id) {
    for(unsigned int i = 0;i < currentNoInstances;i ++) {
        if (instances[i]->instanceId == id) {
            return i;
        }
    }
    return -1;
}

void Model::removeInstance(unsigned int idx) {
    instances.erase(instances.begin() + idx);
    currentNoInstances--;
}

void Model::removeInstance(std::string instanceId) {
    int index = getIdx(instanceId);
    if (index >= 0) {
        removeInstance(index);
    }
}

void Model::initInstances() {
    glm::vec3 *posData = nullptr;
    glm::vec3 *sizeData = nullptr;
    GLenum usage = GL_DYNAMIC_DRAW;
    
    std::vector<glm::vec3> positions , sizes;
    if(States::isActive<unsigned int>(&switches, CONST_INSTANCES)) {
        for(unsigned int i = 0;i < currentNoInstances;i++) {
            positions.push_back(instances[i]->pos);
            sizes.push_back(instances[i]->size);
        }
        if(positions.size() > 0) {
            posData = &positions[0];
            sizeData = &sizes[0];
        }
        usage = GL_STATIC_DRAW;
    }
    
    posVBO = BufferObject(GL_ARRAY_BUFFER);
    posVBO.generate();
    posVBO.bind();
    posVBO.setData<glm::vec3>(UPPER_BOUND, posData, usage);
    
    sizeVBO = BufferObject(GL_ARRAY_BUFFER);
    sizeVBO.generate();
    sizeVBO.bind();
    sizeVBO.setData<glm::vec3>(UPPER_BOUND, sizeData, usage);
    sizeVBO.clear();
    for(int i = 0;i < meshes.size();i++) {
        meshes[i].VAO.bind();
        
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

void Model::loadModel(std::string path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Could not load model at" << path << std::endl << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of("/"));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene){
    // process all meshes
    for(unsigned int i = 0;i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh newMesh = processMesh(mesh, scene);
        boundingRegions.push_back(newMesh.boundRange);
        meshes.push_back(newMesh);
    }
    
    // process all child node
    for(unsigned int i = 0;i < node->mNumChildren; i++) {
        aiNode* childNode = node->mChildren[i];
        processNode(childNode, scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    
    BoundingRegion boundRange(boundType);
    glm::vec3 max_bound(-float(~0));
    glm::vec3 min_bound(float(~0));
    // vertices
    for(unsigned int i = 0; i < mesh->mNumVertices;i ++) {
        Vertex vertex;
        vertex.pos = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z);
        
        for(int j = 0;j < 3;j++) {
            if(max_bound[j] < vertex.pos[j]) max_bound[j] = vertex.pos[j];
            if(min_bound[j] > vertex.pos[j]) min_bound[j] = vertex.pos[j];
        }
        
        vertex.normal = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z);
        
        if(mesh->mTextureCoords[0]) {
            vertex.texCoord = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y);
        } else {
            vertex.texCoord = glm::vec2(0.0f);
        }
        vertices.emplace_back(vertex);
    }
    
    if (boundType == BoudingTypes::AABB) {
        // assign min and max
        boundRange.min = min_bound;
        boundRange.max = max_bound;
        boundRange.ogMin = min_bound;
        boundRange.ogMax = max_bound;
    } else {
        // caculate max distance from the center
        boundRange.center = (min_bound + max_bound) / 2.0f;
        boundRange.ogCenter = boundRange.center;
        float maxRadiusSquared = 0.0f;
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            float radiusSquared = 0.0f; // distance for this vertex
            for (int j = 0; j < 3; j++) {
                radiusSquared += (vertices[i].pos[j] - boundRange.center[j]) * (vertices[i].pos[j] - boundRange.center[j]);
            }
            if (radiusSquared > maxRadiusSquared) {
                // found new squared radius
                // a^2 > b^2 --> |a| > |b|
                maxRadiusSquared = radiusSquared;
            }
        }

        boundRange.radius = sqrt(maxRadiusSquared);
        boundRange.ogRadius = boundRange.radius;
    }
    
    // process indices
    for(unsigned int i = 0;i < mesh->mNumFaces;i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0;j < face.mNumIndices;j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    // process material
//    if(mesh->mMaterialIndex >= 0) {
//        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
//
//        if(!hasTex) {
//            aiColor4D diffuse(1.0f);
//            aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
//
//            aiColor4D specular(1.0f);
//            aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular);
//
//            return Mesh(boundRange, vertices, indices, diffuse, specular);
//        }
//
//        // diffuse maps
//        std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE);
//        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
//
//        // specular maps
//        std::vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR);
//        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
//
//        std::vector<Texture> ambientMaps = loadTextures(material, aiTextureType_EMISSIVE);
//        textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
//    }
//
//    return Mesh(boundRange, vertices, indices, textures);
    
    Mesh ret;
 
    // process material
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        if (States::isActive<unsigned int>(&switches, NO_TEX)) {
            // 1. diffuse colors
            aiColor4D diff(1.0f);
            aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);
            // 2. specular colors
            aiColor4D spec(1.0f);
            aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);
 
            ret = Mesh(boundRange, diff, spec);
        } else {
            // 1. diffuse maps
            std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. specular maps
            std::vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
 
            ret = Mesh(boundRange, textures);
        }
    }
    
 
    ret.loadData(vertices, indices);
    return ret;
}

std::vector<Texture> Model::loadTextures(aiMaterial *mat, aiTextureType type) {
    std::vector<Texture> textures;
    for(unsigned int i = 0;i < mat->GetTextureCount(type);i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::cout << str.C_Str() << std::endl;
        
        // prevent duplicate loading
        bool skip = false;
        for(unsigned int j = 0;j <  textures_loaded.size(); j++) {
            if(std::strcmp(textures_loaded[i].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[i]);
                skip = true;
                break;
            }
        }
        if(!skip) {
            // not loaded yet
            Texture tex(directory, str.C_Str(), type);
            tex.load(false);
            textures.push_back(tex);
            textures_loaded.push_back(tex);
        }
    }
    return textures;
}

void Model::init() {}

void Model::render(Shader &shader, float dt, Scene *scene, bool setModel) {
    if(setModel) {
        shader.setMat4("model", glm::mat4(1.0f));
    }
    std::vector<glm::vec3> positions, sizes;
    if(!States::isActive<unsigned int>(&switches, CONST_INSTANCES)) {
        // update VBO data
        std::vector<glm::vec3> position, sizes;
        bool doUpdate = States::isActive<unsigned int>(&switches, DYNAMIC);
        for(int i = 0; i < currentNoInstances; i++) {
            if(doUpdate) {
                instances[i]->update(dt);
                States::activate(&instances[i]->state, INSTANCE_MOVED);
            } else {
                States::deactivate(&instances[i]->state, INSTANCE_DEAD);
            }
            positions.push_back(instances[i]->pos);
            sizes.push_back(instances[i]->size);
        }
        posVBO.bind();
        posVBO.updateData<glm::vec3>(0, currentNoInstances, &positions[0]);
        
        sizeVBO.bind();
        sizeVBO.updateData<glm::vec3>(0, currentNoInstances, &sizes[0]);
        sizeVBO.clear();
    }
    
    
    shader.setFloat("material.shininess", 0.5f);
    for(unsigned int i = 0, noMeshes = meshes.size();i < noMeshes;i++) {
        meshes[i].render(shader, currentNoInstances);
    }
}

void Model::cleanup() {
    for(auto mesh : meshes) {
        mesh.cleanup();
    }
    posVBO.cleanup();
    sizeVBO.cleanup();
}
