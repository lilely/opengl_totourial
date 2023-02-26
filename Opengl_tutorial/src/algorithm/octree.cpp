//
//  octree.cpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/2/11.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include "octree.hpp"

void Octree::caclulateBounds(BoundingRegion &out, Octant octant, BoundingRegion parentRegion) {
    // find min and max points of corresponding octant
    
    glm::vec3 center = parentRegion.caculateCenter();
    if (octant == Octant::O1) {
        out = BoundingRegion(center, parentRegion.max);
    }
    else if (octant == Octant::O2) {
        out = BoundingRegion(glm::vec3(parentRegion.min.x, center.y, center.z), glm::vec3(center.x, parentRegion.max.y, parentRegion.max.z));
    }
    else if (octant == Octant::O3) {
        out = BoundingRegion(glm::vec3(parentRegion.min.x, parentRegion.min.y, center.z), glm::vec3(center.x, center.y, parentRegion.max.z));
    }
    else if (octant == Octant::O4) {
        out = BoundingRegion(glm::vec3(center.x, parentRegion.min.y, center.z), glm::vec3(parentRegion.max.x, center.y, parentRegion.max.z));
    }
    else if (octant == Octant::O5) {
        out = BoundingRegion(glm::vec3(center.x, center.y, parentRegion.min.z), glm::vec3(parentRegion.max.x, parentRegion.max.y, center.z));
    }
    else if (octant == Octant::O6) {
        out = BoundingRegion(glm::vec3(parentRegion.min.x, center.y, parentRegion.min.z), glm::vec3(center.x, parentRegion.max.y, center.z));
    }
    else if (octant == Octant::O7) {
        out = BoundingRegion(parentRegion.min, center);
    }
    else if (octant == Octant::O8) {
        out = BoundingRegion(glm::vec3(center.x, parentRegion.min.y, parentRegion.min.z), glm::vec3(parentRegion.max.x, center.y, center.z));
    }
}

Octree::Node::Node() : region(BoudingTypes::AABB) {}

// initialize with bounds (no objects yet)
Octree::Node::Node(BoundingRegion bounds) : region(bounds) {}

// initialize with bounds and list of objects
Octree::Node::Node(BoundingRegion bounds, std::vector<BoundingRegion> objectList) : region(bounds) {
    objects.insert(objects.end(), objectList.begin(), objectList.end());
}
     
/*
    functionality
 */

// add instance to pending queue
void Octree::Node::addToPending(RigidBody* instance, Model *model) {
    // get all bounding regions of model and put them in queue

}

// build tree (called during initialization)
void Octree::Node::build() {
    
    BoundingRegion octans[NO_CHILDREN];
    glm::vec3 dimesions = region.caculateCenter();
    std::vector<BoundingRegion> octList[NO_CHILDREN];
    
    if(objects.size() <= 1) {
        goto setVars;
    }
    
    for(int i = 0;i < 3;i++) {
        if(dimesions[i] < MIN_BOUNDS) {
            goto setVars;
        }
    }
    
    for(int i = 0;i < NO_CHILDREN;i ++) {
        caclulateBounds(octans[i], (Octant)(1 << i), region);
    }
    
    for(unsigned int i = 0, len = objects.size();i < len; i++) {
        BoundingRegion br = objects[i];
        for(int j = 0;j < NO_CHILDREN;j++) {
            if(octans[j].containsRegion(br)) {
                octList[j].push_back(br);
                objects.erase(objects.begin() + i);
                i--;
                len--;
                break;
            }
        }
    }
    
    for(int i = 0;i < NO_CHILDREN;i++) {
        if(octList[i].size() > 0) {
            children[i] = new Node(octans[i], octList[i]);
            States::activateIndex(&activeOctants, i);
            children[i]->parent = this;
            children[i]->build();
        }
    }
    
    
    setVars:
    
    treeReady = true;
    treeBuilt = true;
    
    for(int i = 0;i < objects.size();i++) {
        objects[i].cell = this;
    }
    
    return;
}

void Octree::Node::addToPending(RigidBody *instance, trie::Trie<Model *> models) {
    // get all bounding regions of model
    for(BoundingRegion br : models[instance->modelId]->boundingRegions) {
        br.instance = instance;
        br.transform();
        queue.push(br);
    }
}

void Octree::Node::update() {
    if(treeBuilt && treeReady) {
        if(objects.size() == 0) {
            if(!activeOctants) {
                if(currentLifespan == -1) {
                    currentLifespan = maxLifespan;
                } else if(currentLifespan > 0) {
                    currentLifespan--;
                }
            }
        } else {
            if(currentLifespan != -1) {
                if(maxLifespan <= 64) {
                    // extend lifespan becuase of hot pot.
                    maxLifespan <<= 2;
                }
            }
        }
        
        // remove objects that don't exist anymore
        for(int i = 0, listSize = objects.size(); i < listSize; i++ ) {
            if(States::isActive(&objects[i].instance->state, INSTANCE_DEAD)) {
                objects.erase(objects.begin()+i);
                i--;
                listSize--;
            }
        }
        
        std::stack<std::pair<int, BoundingRegion>> movedObjects;
        for(int i = 0, listSize = objects.size(); i < listSize; i++ ) {
            if(States::isActive(&objects[i].instance->state, INSTANCE_MOVED)) {
                movedObjects.push({i, objects[i]});
            }
        }
        
        // remove dead branches
        for(unsigned char flags = activeOctants, i = 0;
            flags > 0;
            flags >>= i, i++) {
            if(States::isIndexActive(&flags, 0) && children[i]->currentLifespan == 0) {
                if(children[i]->objects.size() > 0) {
                    children[i]->currentLifespan = -1;
                } else {
                    free(children[i]);
                    children[i] = nullptr;
                    States::deactivateIndex(&activeOctants, i);
                }
            }
        }
        
        // move moved objects into new nodes
        BoundingRegion movedObj;
        while(movedObjects.size() > 0) {
            /**
            for each moved object
             -tranverse up tree (start with current node) until find a node  that completly enclose the node
             */
            movedObj = movedObjects.top().second;
            Node *current = this;
            while(!current->region.containsRegion(movedObj)) {
                if(current->parent != nullptr) {
                    current = current->parent;
                } else {
                    break;
                }
            }
            
            // remove first object, second object now becomes first
            objects.erase(objects.begin() + movedObjects.top().first);
            movedObjects.pop();
            current->insert(movedObj);
            
            // collistion detection
            // TODO
        }
        
    }
    processPending();
}

// process pending queue
void Octree::Node::processPending() {
    if(!treeBuilt) {
        while(queue.size() > 0) {
            objects.push_back(queue.front());
            queue.pop();
        }
        build();
    } else {
        while(queue.size() > 0) {
            insert(queue.front());
            queue.pop();
        }
    }
    
}

// dynamically insert object into node
bool Octree::Node::insert(BoundingRegion obj) {
    glm::vec3 dimensions = region.caculateDimensions();
    if (objects.size() == 0 ||
        dimensions.x < MIN_BOUNDS ||
        dimensions.y < MIN_BOUNDS ||
        dimensions.z < MIN_BOUNDS
        ) {
        obj.cell = this;
        objects.push_back(obj);
        return true;
    }
    
    // safeguard if object doesn't fit
    if (!region.containsRegion(obj)) {
        return parent == nullptr ? false : parent->insert(obj);
    }
    // create regions if not defined
    BoundingRegion octants[NO_CHILDREN];
    for (int i = 0; i < NO_CHILDREN; i++) {
        if (children[i] != nullptr) {
            // child exists, so take its region
            octants[i] = children[i]->region;
        }
        else {
            // get region for this octant
            caclulateBounds(octants[i], (Octant)(1 << i), region);
        }
    }

    objects.push_back(obj);

    // determine which octants to put objects in
    std::vector<BoundingRegion> octLists[NO_CHILDREN]; // array of list of objects in each octant
    for (int i = 0, len = objects.size(); i < len; i++) {
        objects[i].cell = this;
        for (int j = 0; j < NO_CHILDREN; j++) {
            if (octants[j].containsRegion(objects[i])) {
                octLists[j].push_back(objects[i]);
                // remove from objects list
                objects.erase(objects.begin() + i);
                i--;
                len--;
                break;
            }
        }
    }

    // populate octants
    for (int i = 0; i < NO_CHILDREN; i++) {
        if (octLists[i].size() != 0) {
            // objects exist in this octant
            if (children[i]) {
                for (BoundingRegion br : octLists[i]) {
                    children[i]->insert(br);
                }
            } else {
                // create new node
                children[i] = new Node(octants[i], octLists[i]);
                children[i]->parent = this;
                States::activateIndex(&activeOctants, i);
                children[i]->build();
            }
        }
    }
    
    return true;
}

// destroy object (free memory)
void Octree::Node::drestroy() {
    if(children != nullptr) {
        for(int flags = activeOctants, i = 0;
            flags > 0;
            flags >>= 1, i++) {
            if(States::isIndexActive(&flags, 0)){
                if(children[i] != nullptr) {
                    children[i]->drestroy();
                    children[i] = nullptr;
                }
            }
        }
    }
    objects.clear();
    while(queue.size() > 0) {
        queue.pop();
    }
}
