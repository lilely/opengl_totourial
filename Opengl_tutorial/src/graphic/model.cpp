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

Model::Model(glm::vec3 pos, glm::vec3 size, bool hasTex, BoudingTypes boundType) : boundType(boundType), size(size), hasTex(hasTex) {
    rb.pos = pos;
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
        meshes.push_back(processMesh(mesh, scene));
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
    } else {
        // caculate max distance from the center
        boundRange.center = (min_bound + max_bound) / 2.0f;
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
//        if (States::isActive<unsigned char>(&switches, NO_TEX)) {
        if(!hasTex) {
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

void Model::render(Shader &shader, float dt, bool setModel, bool doRender, Box *box) {
    rb.update(dt);
    if(setModel) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model,rb.pos);
        model = glm::scale(model,size);
    //        model = glm::rotate(model,(float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.5f));
        shader.setMat4("model", model);
    }
    shader.setFloat("material.shininess", 0.5f);
    for(auto mesh : meshes) {
        mesh.render(shader, box, rb.pos, size, doRender);
    }
}

void Model::cleanup() {
    for(auto mesh : meshes) {
        mesh.cleanup();
    }
}
