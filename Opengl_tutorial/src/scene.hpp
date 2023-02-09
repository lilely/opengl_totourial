//
//  scene.hpp
//  Opengl_tutorial
//
//  Created by 星金 on 2023/2/9.
//  Copyright © 2023 com.corp.jinxing. All rights reserved.
//

#ifndef scene_hpp
#define scene_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphic/shader.hpp"
#include "io/camera.hpp"
#include "graphic/lights/light.hpp"

#include <glm/glm.hpp>


#include <vector>

class Scene {
public:
    
    static void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
    
    Scene(){};
    
    Scene(int glfwVersionMajor, int glfwVersionMinor, const char *title, unsigned int scrWidth, unsigned int scrHeight);
    
    bool init();
    
    /*
        main loop methods
     */
    
    void processInput(float dt);
    
    void update();
    
    void newFrame();
    
    void render(Shader shader, bool applyLighting = true);
    
    /* clean up methods */
    void cleanup();
    
    /* accessors */
    Camera* getActiveCamera();
    
    bool shouldClose();
    
    /* modifiers */
    
    void setShouldClose(bool shouldClose);
    
    void setWindowColor(float r, float g, float b, float a);
    
    void setParamters();
    
    /* lights */
    
    std::vector<PointLight*> pointLights;
    unsigned int activePointLights;
    
    std::vector<SpotLight*> spotLights;
    unsigned int activeSpotLights;
    
    DirLight* dirLight;
    bool dirLightActive;
    
    /* Cameras */
    std::vector<Camera *>cameras;
    unsigned int activeCamera;
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 cameraPos;
    
protected:
    // window object
    GLFWwindow* window;
    
    // window vals
    const char *title;
    
    static unsigned int SCR_WIDTH;
    static unsigned int SCR_HEIGHT;
    
    float bg[4]; // background color
    
    // GLFW info
    int glfwVersionMajor;
    int glfwVersionMinor;
    
};

#endif /* scene_hpp */
