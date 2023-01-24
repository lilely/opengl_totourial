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

int Texture::currentId = 0;

Texture::Texture() {}

Texture::Texture(const char *_path, const char*_name, bool defaultParams) : path(_path), name(_name) ,id(++currentId){
    generate();
    if(defaultParams) {
        setWrap(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
        setFilters(GL_LINEAR);
    }
}

void Texture::generate() {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
}

void Texture::load(bool flip) {
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load(path, &width, &height, &nChannels, 0);
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
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Texture::setFilters(GLenum all) {
    setFilters(all,all);
}

void Texture::setFilters(GLenum mag, GLenum min) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
}

void Texture::setWrap(GLenum all) {
    setWrap(all,all);
}

void Texture::setWrap(GLenum s, GLenum t) {
    // set the texture wrapping parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);    // set texture wrapping to GL_REPEAT (default wrapping method)
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
}

void Texture::activate() {
    glActiveTexture(GL_TEXTURE0 + id);
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, id);
}
