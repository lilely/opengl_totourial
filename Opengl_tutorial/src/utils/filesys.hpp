//
//  filesys.hpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/26.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef filesys_hpp
#define filesys_hpp

#include <string>

class FileManager {
public:
    static std::string getRootPath();
    static std::string getFilePath(const char *filename);
};

#endif /* filesys_hpp */
