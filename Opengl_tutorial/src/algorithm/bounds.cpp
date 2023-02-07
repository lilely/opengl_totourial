//
//  bounds.cpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/2/6.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include "bounds.hpp"

BoudingRegion::BoudingRegion(BoudingTypes type) : type(type){}

BoudingRegion::BoudingRegion(glm::vec3 center, float radius) : type(BoudingTypes::SPHERE), center(center) , radius(radius) {}

BoudingRegion::BoudingRegion(glm::vec3 min, glm::vec3 max) : type(BoudingTypes::AABB), min(min), max(max) {}

glm::vec3 BoudingRegion::caculateCenter() {
    return type == BoudingTypes::AABB ? (max + min) / 2.0f : center;
}

glm::vec3 BoudingRegion::caculateDimensions() {
    return type == BoudingTypes::AABB ? (max - min) : glm::vec3(radius * 2);
}

bool BoudingRegion::containsPoint(glm::vec3 point) {
    if(type == BoudingTypes::AABB) {
        return (point.x > min.x && point.x < max.x) &&
        (point.y > min.y && point.y < max.y) &&
        (point.z > min.z && point.z < max.z);
    } else {
        return glm::distance(point, center) < radius;
    }
}

bool BoudingRegion::containsRegion(BoudingRegion br) {
    if(br.type == BoudingTypes::AABB) {
        return containsPoint(br.min) && containsPoint(br.max);
    } else if(type == BoudingTypes::SPHERE && br.type == BoudingTypes::SPHERE) {
        // if both spheres, combination of distance from centers and br.radius is less than radius
        return glm::distance(br.center, center) + radius < radius;
    } else {
        // if this is a box and br is a shpere
        
        if(!containsPoint(br.center)) {
            // center is outside of box
            return false;
        }
        
        for(int i = 0;i < 3;i++) {
            if(abs(max[i] - br.center[i]) < br.radius ||
               abs(br.center[i] - min[i]) < br.radius) {
                return false;
            }
        }
        return true;
    }
}

bool BoudingRegion::intersectsWith(BoudingRegion br) {
    
    if(type == BoudingTypes::AABB && br.type == BoudingTypes::AABB) {
        // both boxes
        
        glm::vec3 rad = caculateDimensions();
        glm::vec3 radBr = br.caculateDimensions();
        
        glm::vec3 ctr = caculateCenter();
        glm::vec3 brCtr = br.caculateCenter();
        
        for(int i = 0;i < 3;i++) {
            if(abs(ctr[i] - brCtr[i]) > rad[i] + radBr[i]) {
                return false;
            }
        }
        return true;
    } else if(type == BoudingTypes::SPHERE && br.type == BoudingTypes::SPHERE) {
        // Both sphere
        return glm::distance(center, br.center) < radius + br.radius;
    } else if(type == BoudingTypes::SPHERE) {
        float distSquared = 0.0f;
        for(int i = 0;i < 3;i++) {
            if(center[i] < br.min[i]) {
                distSquared += (br.min[i] - center[i]) * (br.min[i] - center[i]);
            } else if(center[i] > br.max[i]) {
                distSquared += (center[i] - br.max[i]) * (center[i] - br.max[i]);
            }
        }
        
        return distSquared < radius * radius;
    } else {
        // this is a box, br is a shpere
        return br.intersectsWith(*this);
    }
}
