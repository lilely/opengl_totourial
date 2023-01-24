//
//  model.cpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/24.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include "model.hpp"

void Model::init() {}

void Model::render(Shader &shader) {
    for(auto mesh : meshes) {
        mesh.render(shader);
    }
}

void Model::cleanup() {
    for(auto mesh : meshes) {
        mesh.cleanup();
    }
}
