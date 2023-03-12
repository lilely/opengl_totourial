//
//  octree.hpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/2/11.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef octree_hpp
#define octree_hpp

#include <vector>
#include <queue>
#include <stack>

#include "list.hpp"
#include "states.hpp"
#include "bounds.hpp"
#include "../physics/rigidbody.hpp"
#include "../graphic/model.hpp"
#include "../graphic/models/box.hpp"

#define NO_CHILDREN 8
#define MIN_BOUNDS 0.5

class Model;

class BoundingRegion;

namespace Octree {
    /*
        enum to represent octants
    */

    enum class Octant : unsigned char {
        O1 = 0x01,    // = 0b00000001
        O2 = 0x02,    // = 0b00000010
        O3 = 0x04,    // = 0b00000100
        O4 = 0x08,    // = 0b00001000
        O5 = 0x10,    // = 0b00010000
        O6 = 0x20,    // = 0b00100000
        O7 = 0x40,    // = 0b01000000
        O8 = 0x80    // = 0b10000000
    };
    
    void caclulateBounds(BoundingRegion &out, Octant octant, BoundingRegion parentRegion);

    class Node {
    public:
        // parent pointer
        Node *parent;
        // array of children (8)
        std::vector<Node *>children;
        
        // switch for active octants
        unsigned char activeOctants;
        
        // if tree is ready
        bool treeReady = false;
        // if tree is built
        bool treeBuilt = false;
        
        short maxLifespan = 8;
        short currentLifespan = -1;
        
        // list of objects in node
        std::vector<BoundingRegion> objects;
        // queue of objects to be dynamically inserted
        std::queue<BoundingRegion> queue;
        
        // Region of bounds of cell (AABB)
        BoundingRegion region;
        
        /*
            Construtors
         */
        Node();
        
        // initialize with bounds (no objects yet)
        Node(BoundingRegion bounds);
        
        // initialize with bounds and list of objects
        Node(BoundingRegion bounds, std::vector<BoundingRegion> objectList);
        
        void addToPending(RigidBody *instance, trie::Trie<Model *> models);
        
        /*
            functionality
         */
        void addToPending(RigidBody* instance, Model *model);
        
        // build tree (called during initialization)
        void build();
        
        void update(Box &box);
        
        // process pending queue
        void processPending();
        
        // dynamically insert object into node
        bool insert(BoundingRegion obj);
        
        void checkCollisionSelf(BoundingRegion obj);
        
        void checkCollisionChildren(BoundingRegion obj);
        
        // destroy object (free memory)
        void drestroy();
        
    };

};

#endif /* octree_hpp */
