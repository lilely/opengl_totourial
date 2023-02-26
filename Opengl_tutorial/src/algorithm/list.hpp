//
//  list.h
//  Opengl_tutorial
//
//  Created by 星金 on 2023/2/11.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef list_h
#define list_h

/*
    namespace to tie together list utility methods
*/

namespace List {
    // find index of item in vector (list)
    template<typename T>
    int getIndexOf(std::vector<T> v, T x) {
        return std::find(v.begin(), v.end(), x) - v.begin();
    }

    // test if list contains item
    template<typename T>
    bool contains(std::vector<T> v, T x) {
        return std::find(v.begin(), v.end(), x) != v.end();
    }
};

#endif /* list_h */
