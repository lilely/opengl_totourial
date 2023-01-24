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

#include <vector>

#include "mesh.hpp"

class Model {
public:
    std::vector<Mesh> meshes;
    
    void init();
    
    void render(Shader &shader);
    
    void cleanup();
};

#endif /* model_hpp */
