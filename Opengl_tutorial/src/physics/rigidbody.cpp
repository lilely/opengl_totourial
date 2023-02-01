//
//  rigidBody.cpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/2/1.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include "rigidbody.hpp"

RigidBody::RigidBody(float mass, glm::vec3 pos, glm::vec3 velocity, glm::vec3 acceleration) : mass(mass),
    pos(pos),
    velocity(velocity),
    acceleration(acceleration){}

void RigidBody::update(float dt) {
    pos += (dt * velocity + (0.5f * dt * dt) * acceleration);
    velocity += dt * acceleration;
}
