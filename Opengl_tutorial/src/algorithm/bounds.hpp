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

// forward declaration
namespace Octree {
    class Node;
};

enum BoudingTypes : unsigned char {
    AABB = 0x01,
    SPHERE = 0x02,
};

class BoundingRegion {
public:
    BoudingTypes type;
    
    // Cell
    Octree::Node *cell;
    
    // Sphere region
    glm::vec3 center;
    float radius;
    
    // Cube region
    glm::vec3 min;
    glm::vec3 max;
    
    BoundingRegion(BoudingTypes type = BoudingTypes::AABB);
    
    BoundingRegion(glm::vec3 center, float radius);
    
    BoundingRegion(glm::vec3 min, glm::vec3 max);
    
    glm::vec3 caculateCenter();
    
    glm::vec3 caculateDimensions();
    
    bool containsPoint(glm::vec3);
    
    bool containsRegion(BoundingRegion br);
    
    bool intersectsWith(BoundingRegion br);
    
    bool operator==(BoundingRegion value);
    
private:
    
};


#endif /* bounds_hpp */
