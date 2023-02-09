//
//  vertexmemory.hpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/2/7.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef vertexmemory_hpp
#define vertexmemory_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <map>

class BufferObject {
public:
    // value/location
    GLuint val;
    
    // type of buffer (GL_ARRAY_BUFFER || GL_ELEMENT_ARRAY_BUFFER, etc)
    GLenum type;
    
    BufferObject(GLenum type = GL_ARRAY_BUFFER) : type(type) {}
    
    void generate() {
        glGenBuffers(1, &val);
    }
    
    void bind() {
        glBindBuffer(type, val);
    }
    
    template<typename T>
    void setData(GLuint noElements, T*data, GLenum usage) {
        glBufferData(type, noElements * sizeof(T), data, usage);
    }
    
    template<typename T>
    void updateData(GLintptr offset, GLuint noElements, T*data) {
        glBufferSubData(type, offset, noElements * sizeof(T), data);
    }
    
    template<typename T>
    void setAttPointer(GLuint idx, GLint size, GLenum type, GLuint stride, GLuint offset, GLuint divisor = 0) {
        glVertexAttribPointer(idx, size, type, GL_FALSE, stride * sizeof(T), (void *)(offset));
        glEnableVertexAttribArray(idx);
        if(divisor > 0) {
            glVertexAttribDivisor(idx, divisor);
        }
    }
    
    void clear() {
        glBindBuffer(type, 0);
    }
    
    void cleanup() {
        glDeleteBuffers(1, &val);
    }
};

/*
    class for array objects
    - VAO
 **/
class ArrayObject {
public:
    // value/location
    GLuint val;
    
    // map of names to buffers
    std::map<const char*, BufferObject> buffers;
    
    BufferObject& operator[](const char* key) {
        return buffers[key];
    }
    
    void generate(){
        glGenVertexArrays(1, &val);
    }
    
    void bind() {
        glBindVertexArray(val);
    }
    
    // draw arrays
    void draw(GLenum mode, GLuint first, GLuint count) {
        glDrawArrays(mode, first, count);
    }
    
    // draw
    void draw(GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instancecount = 1) {
        glDrawElementsInstanced(mode, count, type, (void *)indices, instancecount);
    }
    
    void cleanup() {
        glDeleteVertexArrays(1, &val);
        for(auto p : buffers) {
            p.second.cleanup();
        }
    }
    
    static void clear() {
        glBindVertexArray(0);
    }
};

#endif /* vertexmemory_hpp */
