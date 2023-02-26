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
#include "../../scene.hpp"

class Gun : public Model
{
public:
    Gun() : Model("Gun", BoudingTypes::AABB, 100, CONST_INSTANCES | NO_TEX) {}

    void render(Shader &shader, float dt, Scene &scene) {
        rb.update(dt);
        glm::mat4 model = glm::mat4(1.0f);
        
        rb.pos += scene.getActiveCamera()->cameraPos + glm::vec3(scene.getActiveCamera()->cameraFront * 2.5f) + glm::vec3(scene.getActiveCamera()->cameraUp * -0.8f);
        model = glm::translate(model,rb.pos);
        model = glm::scale(model,size);
        
        float theta = acos(glm::dot(scene.getActiveCamera()->worldUp, scene.getActiveCamera()->cameraFront) / glm::length(scene.getActiveCamera()->cameraUp) / glm::length(scene.getActiveCamera()->cameraFront));
        model = glm::rotate(model, atanf(1) * 2 - theta, scene.getActiveCamera()->cameraRight);
        
        glm::vec2 front2d = glm::vec2(scene.getActiveCamera()->cameraFront.x, scene.getActiveCamera()->cameraFront.z);
        theta = acos(glm::dot(glm::vec2(1.0f, 0.0f), front2d) / glm::length(front2d));
        model = glm::rotate(model, scene.getActiveCamera()->cameraFront.z < 0 ? theta : -theta, scene.getActiveCamera()->worldUp);
        
        shader.setMat4("model", model);
        Model::render(shader, dt, &scene, false);
    }
};

#endif /* gun_h */
