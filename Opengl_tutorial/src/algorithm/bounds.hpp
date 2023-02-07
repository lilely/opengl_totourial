//
//  bounds.hpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/2/6.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef bounds_hpp
#define bounds_hpp

#include <glm/glm.hpp>

enum BoudingTypes {
    AABB,
    SPHERE,
};

class BoudingRegion {
public:
    BoudingTypes type;
    
    // Sphere region
    glm::vec3 center;
    float radius;
    
    // Cube region
    glm::vec3 min;
    glm::vec3 max;
    
    BoudingRegion(BoudingTypes type);
    
    BoudingRegion(glm::vec3 center, float radius);
    
    BoudingRegion(glm::vec3 min, glm::vec3 max);
    
    glm::vec3 caculateCenter();
    
    glm::vec3 caculateDimensions();
    
    bool containsPoint(glm::vec3);
    
    bool containsRegion(BoudingRegion br);
    
    bool intersectsWith(BoudingRegion br);
    
private:
    
};


#endif /* bounds_hpp */
