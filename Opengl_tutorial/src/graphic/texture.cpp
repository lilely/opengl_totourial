//
//  texture.cpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/24.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include "texture.hpp"
#include <iostream>
#include <stb/stb_image.h>

Texture::Texture() {}

Texture::Texture(std::string dir, std::string path, aiTextureType type) : dir(dir), path(path), type(type) {
    generate();
}

void Texture::generate() {
    glGenTextures(1, &id);
}

void Texture::load(bool flip) {
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    int width, height, nChannels;
    unsigned char *data = stbi_load((dir + "/" + path).c_str(), &width, &height, &nChannels, 0);
    
    GLenum colorMode = GL_RGB;
    switch (nChannels) {
        case 1:
            colorMode = GL_RED;
            break;
        case 4:
            colorMode = GL_RGBA;
            break;
    }
    if (data) {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Texture::activate() {
    glActiveTexture(GL_TEXTURE0 + id);
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, id);
}
