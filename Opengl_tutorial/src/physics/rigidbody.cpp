//
//  rigidBody.cpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/2/1.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include "rigidbody.hpp"

RigidBody::RigidBody() {}

RigidBody::RigidBody(std::string modelId, glm::vec3 size, float mass, glm::vec3 pos) : modelId(modelId), size(size), mass(mass),
    pos(pos),
    velocity(glm::vec3(0.0f)),
    acceleration(0.0f){}

void RigidBody::update(float dt) {
    pos += (dt * velocity + (0.5f * dt * dt) * acceleration);
    velocity += dt * acceleration;
}

void RigidBody::applyForce(glm::vec3 force) {
    acceleration += force / mass;
}

void RigidBody::applyForce(glm::vec3 direction, float magnitude) {
    applyForce(direction * magnitude);
}

void RigidBody::applyAcceleration(glm::vec3 a) {
    acceleration += a;
}

void RigidBody::applyAcceleration(glm::vec3 direction, float magnitude) {
    applyAcceleration(direction * magnitude);
}

void RigidBody::applyImpulse(glm::vec3 force, float dt) {
    velocity += force / mass * dt;
}

void RigidBody::applyImpulse(glm::vec3 direction, float magnitude, float dt) {
    applyImpulse(direction * magnitude, dt);
}

void RigidBody::transferEnergy(float joules) {
    if(joules == 0.0f) {
        return;
    }
    float deltaV = sqrt(2 * abs(joules) / mass);
    velocity += joules > 0 ? deltaV : -deltaV;
}

void RigidBody::transferEnergy(float joules, glm::vec3 direction) {
    if (joules == 0) {
        return;
    }

    // comes from formula: KE = 1/2 * m * v^2
    glm::vec3 deltaV = sqrt(2 * abs(joules) / mass) * direction;

    velocity += joules > 0 ? deltaV : -deltaV;
}
