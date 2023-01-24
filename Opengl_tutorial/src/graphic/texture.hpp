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

class Texture {
public:
    Texture();
    Texture(const char *path, const char* name, bool defaultParams);
    void generate();
    void load(bool flip = false);
    
    void setFilters(GLenum all);
    void setFilters(GLenum mag, GLenum min);
    
    void setWrap(GLenum all);
    void setWrap(GLenum s, GLenum t);
    
    void activate();
    void bind();
    
    int id;
    unsigned int tex;
    const char* name;
private:
    static int currentId;
    const char* path;
    int width;
    int height;
    int nChannels;
    
};

#endif /* texture_hpp */
