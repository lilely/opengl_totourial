//
//  rigidbody.hpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/2/1.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef rigidbody_hpp
#define rigidbody_hpp

#include <glm/glm.hpp>

class RigidBody {
    
public:
    float mass;
    glm::vec3 pos;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    
    RigidBody(float mass = 0.0f, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 acceleration = glm::vec3(0.0f));
    
    void update(float dt);
    
};

#endif /* rigidBody_hpp */
