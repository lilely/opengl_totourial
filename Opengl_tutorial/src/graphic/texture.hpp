//
//  texture.hpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/24.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef texture_hpp
#define texture_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assimp/scene.h>

class Texture {
public:
    Texture();
    Texture(std::string dir, std::string path, aiTextureType type);
    
    void generate();
    void load(bool flip = false);

    void activate();
    void bind();
    
    unsigned int id;
    aiTextureType type;
    std::string dir;
    std::string path;
};

#endif /* texture_hpp */
