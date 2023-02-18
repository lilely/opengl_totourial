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
#include <memory>
#include <glm/glm.hpp>
#include "graphic/model.hpp"


#include <vector>

// Forword delcaration
class Model;

class Scene {
public:
    std::map<std::string, Model*> models;
    std::map<std::string, std::pair<std::string, unsigned int>> instances;
    
    static void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
    
    Scene(int glfwVersionMajor, int glfwVersionMinor, const char *title, unsigned int scrWidth, unsigned int scrHeight, std::string currentId = "aaaaaaaa");
    
    bool init();
    
    /*
        main loop methods
     */
    
    void processInput(float dt);
    
    void update();
    
    void newFrame();
    
    void render(Shader shader, bool applyLighting = true);
    
    void renderInstance(std::string modelId, Shader shader, float dt);
    
    /*
        Model/instance methods
     */
    
    void registerModel(Model *model);
    
    std::string generateInstance(std::string modelId, glm::vec3 size, float mass, glm::vec3 pos);
    
    void initInstances();
    
    void loadModels();
    
    void removeInstance(std::string instanceId);
    
    std::string currentId;
    std::string generateId();
    
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
    
    std::vector<std::shared_ptr<PointLight>> pointLights;
    unsigned int activePointLights;
    
    std::vector<std::shared_ptr<SpotLight>> spotLights;
    unsigned int activeSpotLights;
    
    std::shared_ptr<DirLight> dirLight;
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
    
    bool needSpotLight;
    
};

#endif /* scene_hpp */
