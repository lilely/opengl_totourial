//
//  filesys.cpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/1/26.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#include "filesys.hpp"
#include "CoreFoundation/CoreFoundation.h"
#include <unistd.h>

std::string FileManager::getRootPath() {
//    const int MAXPATH = 250;
//    char buffer[MAXPATH];
//    strcpy(buffer, getenv("HOME"));
////    std::string ret(buffer);
//    CFBundleRef mainBundle = CFBundleGetMainBundle();
//    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
//    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)buffer, PATH_MAX)) {
//        // error!
//    }
//    CFRelease(resourcesURL);
//    chdir(buffer);
    
    // "/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/object.vs.glsl"

    return "/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset";
}

std::string FileManager::getFilePath(const char *filename) {
    if(filename == nullptr) {
        return "";
    }
    return FileManager::getRootPath() + "/" + std::string(filename);
}
