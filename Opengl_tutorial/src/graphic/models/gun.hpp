//
//  gun.hpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/29.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef gun_h
#define gun_h

#include "../model.hpp"
#include "../../io/camera.hpp"

class Gun : public Model
{
public:
    Gun() : Model(glm::vec3(0.0f), glm::vec3(0.05f), false) {}
    
    void render(Shader &shader) {
        glm::mat4 model = glm::mat4(1.0f);
        
        pos = Camera::defaultCamera.cameraPos + glm::vec3(Camera::defaultCamera.cameraFront * 2.5f) + glm::vec3(Camera::defaultCamera.cameraUp * -0.8f);
        model = glm::translate(model,pos);
        model = glm::scale(model,size);
    //        model = glm::rotate(model,(float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.5f));
        
        float theta = acos(glm::dot(Camera::defaultCamera.worldUp, Camera::defaultCamera.cameraFront) / glm::length(Camera::defaultCamera.cameraUp) / glm::length(Camera::defaultCamera.cameraFront));
        model = glm::rotate(model, atanf(1) * 2 - theta, Camera::defaultCamera.cameraRight);
        
        glm::vec2 front2d = glm::vec2(Camera::defaultCamera.cameraFront.x, Camera::defaultCamera.cameraFront.z);
        theta = acos(glm::dot(glm::vec2(1.0f, 0.0f), front2d) / glm::length(front2d));
        model = glm::rotate(model, Camera::defaultCamera.cameraFront.z < 0 ? theta : -theta, Camera::defaultCamera.worldUp);
        
        shader.setMat4("model", model);
        Model::render(shader, false);
    }
};

#endif /* gun_h */
