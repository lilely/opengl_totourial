#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "graphic/shader.hpp"
#include "io/keyboard.hpp"
#include "io/mouse.hpp"
#include "io/joystick.hpp"
#include "io/camera.hpp"
#include "io/screen.hpp"
#include "graphic/texture.hpp"
#include <iostream>
#include "graphic/models/cube.hpp"
#include "graphic/model.hpp"
#include "graphic/material.hpp"
#include "graphic/models/lamp.hpp"
#include "graphic/lights/light.hpp"
#include <vector>
#include <stack>
#include "graphic/models/gun.hpp"
#include "graphic/models/sphere.hpp"
#include "physics/enviroment.hpp"
#include "graphic/models/box.hpp"
#include "scene.hpp"
#include "algorithm/states.hpp"

void processInput(float delta);

// settings

float mixVal = 0.5f;

glm::mat4 transform = glm::mat4(1.0f);

Joystick mainJ(0);

bool needSpotLight = false;

Scene scene(3, 3, "opengl_tutorial", 800, 600);

int activeCamera = 0;

Sphere sphere(10);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    scene.init();
    scene.cameras.push_back(&Camera::defaultCamera);
    scene.cameras.emplace_back(new Camera(glm::vec3(0.0f, 0.0f, 7.0f)));
    scene.activeCamera = 0;
    
    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    

#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    float lastTime = glfwGetTime();
    
    /*
        Shaders
     */
    // Object Shader
    Shader instanceShader("/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/instance/instance.vs.glsl", "/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/object_texture.fs.glsl");
    
    // Light Shader
    Shader lampShader("/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/instance/instance.vs.glsl","/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/lamp.fs.glsl");
    
    // Box Shader
    Shader boxShader("/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/instance/box.vs.glsl","/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/instance/box.fs.glsl");
    
    
    /*
        Models=================
     */
    Lamp lamp(4);
    lamp.init();
    scene.registerModel(&lamp);
    sphere.init();
    scene.registerModel(&sphere);
    Model tyrannosarus("Tyrannosarus", BoudingTypes::AABB, 3, CONST_INSTANCES);
    tyrannosarus.loadModel("/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/models/tyrannosarus/scene.gltf");
    tyrannosarus.init();
    scene.registerModel(&tyrannosarus);
    scene.generateInstance(tyrannosarus.id, glm::vec3(1.0f), 1.0f, glm::vec3(8.0f,0.0f,0.0f));
     
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };
    
    for (unsigned int i = 0; i < 4; i++) {
        std::shared_ptr<PointLight> pl = std::make_shared<PointLight>(pointLightPositions[i], 1.0f, 0.07f, 0.05f, glm::vec3(0.5f),glm::vec3(0.5f),glm::vec3(0.5f));
        scene.generateInstance(lamp.id, glm::vec3(0.25f), 0.25f, pl->position);
        scene.pointLights.push_back(pl);
        States::activateIndex(&scene.activePointLights, i);
    }
    
//    Model model(glm::vec3(8.0f,0.0f,0.0f), glm::vec3(1.0f), true);
//    model.loadModel("/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/models/tyrannosarus/scene.gltf");
//    model.init();
    
    Box box;
    box.init();
    
    scene.initInstances();
//
//    Gun gun;
//    gun.loadModel("/Users/xingjin/Projects/MacProject/opengl_totourial/Opengl_tutorial/asset/models/m4a1/scene.gltf");
    
    scene.getActiveCamera()->updateCameraPos(CameraDirection::BACKWARD, 5.0f);
    scene.dirLight = std::make_shared<DirLight>(glm::vec3(-0.2, -1.0f, -0.3f),
                                                glm::vec3(1.0f), glm::vec3(0.4f),
                                                glm::vec3(0.75f));
    
    SpotLight aSpotLight({
        scene.getActiveCamera()->cameraPos,
        scene.getActiveCamera()->cameraFront,
        1.0f,
        0.07f,
        0.005f,
        glm::cos(glm::radians(12.5f)),
        glm::cos(glm::radians(15.5f)),
        glm::vec3(1.0f),
        glm::vec3(1.0f),
        glm::vec3(1.0f)
    });
    
    auto spotLight = std::make_shared<SpotLight>(aSpotLight);
    scene.spotLights.push_back(spotLight);
    States::activateIndex(&scene.activeSpotLights, (unsigned int)0);
    
    if(mainJ.isPresent()) {
        mainJ.update();
        std::cout << "joystick is preseted!" << std::endl;
    }

    // render loop
    // -----------
    while (!scene.shouldClose())
    {
        float currentTime = glfwGetTime();
        float delta = currentTime - lastTime;
        lastTime = currentTime;
        
        // clear box vertices and sizes
        box.offsetVecs.clear();
        box.sizeVecs.clear();
        // input
        // -----
        scene.processInput(delta);
        processInput(delta);
        // render
        // ------
        scene.update();
        // bind textures on corresponding texture units
    
//        ourShader.activate();
//        ourShader.setFloat3("viewPos", cameras[activeCamera]->cameraPos);
        
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        
        view = scene.getActiveCamera()->getViewMatrix();
        projection = glm::perspective(scene.getActiveCamera()->getZoom(),(float)Screen::SCR_WIDTH/(float)Screen::SCR_HEIGHT, 0.01f, 1000.0f);
        
        // Dir Lights
        scene.dirLight->direction = glm::vec3(glm::rotate(glm::mat4(1.0f), 0.05f, glm::vec3(1.0f,0.0f,0.0f)) * glm::vec4(scene.dirLight->direction,1.0f));
        
        if(States::isIndexActive(&scene.activeSpotLights, (unsigned int)0)) {
            // Spot Lights
            spotLight->position = scene.getActiveCamera()->cameraPos;
            spotLight->direction = scene.getActiveCamera()->cameraFront;
        }

        for(int i = 0; i < sphere.currentNoInstances;i++) {
            if(glm::length(sphere.instances[i]->pos - scene.getActiveCamera()->cameraPos) > 250.f) {
                scene.markForDeletion(sphere.instances[i]->instanceId);
            }
        }
        
        // render lanuch objcets
        scene.render(instanceShader);
        scene.renderInstance(sphere.id, instanceShader, delta);
        scene.renderInstance(tyrannosarus.id, instanceShader, delta);
        
        scene.render(lampShader);
        scene.renderInstance(lamp.id, lampShader, delta);

        if(box.offsetVecs.size() > 0) {
            scene.render(boxShader, false);
            box.render(boxShader, delta);
        }
        
        scene.clearDeadInstances();
        scene.newFrame();
    }


//    model.cleanup();
//    gun.cleanup();
//    shpere.cleanup();
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    box.cleanup();
    
    scene.cleanup();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    
    return 0;
    
}

void addSphere() {
    RigidBody *rb = scene.generateInstance(sphere.id, glm::vec3(0.25f), 1.0f, scene.getActiveCamera()->cameraPos);
    if(rb != nullptr) {
        // instance generated
        rb->transferEnergy(300.0f, scene.getActiveCamera()->cameraFront);
        rb->applyAcceleration(Enviroment::gravitationalAcceleration);
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(float delta)
{
    if(Keyboard::keyWentDown(GLFW_KEY_F)) {
        addSphere();
    }
    
}

