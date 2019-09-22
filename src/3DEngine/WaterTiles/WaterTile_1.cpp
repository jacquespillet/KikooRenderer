#include "WaterTile_1.hpp"

#include "WaterTileShader_1.hpp"
#include "../BaseObjects.hpp"
#include "../Components/MaterialComponent.hpp"


namespace KikooRenderer {

namespace CoreEngine {

WaterTile_1::WaterTile_1(std::string name, Scene* scene) : Object3D(name, scene) {
    waterShader = GetWaterTile_1Shader();
    
    quad = GetQuad(scene, "particle", glm::dvec3(0, 0.2, 0), glm::dvec3(90, 0, 0), glm::dvec3(20), glm::dvec4(0.5, 0.5, 0.5, 1));
    quad->Start();
    quad->Enable();

    quadMaterial = (MaterialComponent*) quad->GetComponent("Material");
    quadMaterial->SetShader(&waterShader);
    reflectionFramebuffer = new Framebuffer(1024, 1024,  GL_RGBA16F, GL_RGBA, GL_FLOAT, true, true, false);
    refractionFramebuffer = new Framebuffer(1024, 1024,  GL_RGBA16F, GL_RGBA, GL_FLOAT, true, true, false);

    dudvMap = Texture("C:/Users/Jacques/Pictures/Textures/WaterTiles/1/waterDUDV (1).png", GL_TEXTURE2);
    normalMap = Texture("C:/Users/Jacques/Pictures/Textures/WaterTiles/1/matchingNormalMap.png", GL_TEXTURE3);
}

void WaterTile_1::WindowResize(int w, int h) {
    reflectionFramebuffer = new Framebuffer(w, h,  GL_RGBA16F, GL_RGBA, GL_FLOAT, true, true, false);
    refractionFramebuffer = new Framebuffer(w, h,  GL_RGBA16F, GL_RGBA, GL_FLOAT, true, true, false);
}


void WaterTile_1::Start() {
	started = true;
}

void WaterTile_1::Enable() {
	enabled = true;
}

void WaterTile_1::Update() {
    // scene->triggerRefresh = true;
}

void WaterTile_1::Render(glm::mat4* overrideViewMatrixp) {
    GETGL
    ogl->glEnable(GL_CLIP_DISTANCE0);
    //1. Render reflection
    //____________________________________________
    float distance = 2 * scene->camera->transform->position.y;
    scene->camera->transform->position.y -= distance;
    scene->camera->transform->rotation.x = -scene->camera->transform->rotation.x; 

    reflectionFramebuffer->Enable();    
    ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
    ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);

    //Render all objects
    for(int i=0; i<scene->objects3D.size(); i++) {
        if(scene->objects3D[i] && scene->objects3D[i]->visible && scene->objects3D[i] != this) {
            MaterialComponent* material = (MaterialComponent*) scene->objects3D[i]->GetComponent("Material");
            if(material != nullptr) {
                int program = material->shader->programShaderObject;
                ogl->glUseProgram(program);
                GLuint loc = ogl->glGetUniformLocation(program, "clipPlane");
                ogl->glUniform4fv(loc, 1, glm::value_ptr(glm::vec4(0, 1, 0, -this->transform->position.y)));
                scene->objects3D[i]->Render();
            }
        }
    }

    if(scene->hasSkybox) {
        ogl->glDepthFunc(GL_LEQUAL);        
        scene->skyboxCube->Render();
        ogl->glDepthFunc(GL_LESS);
    }
    
    
    scene->camera->transform->position.y  += distance;
    scene->camera->transform->rotation.x = -scene->camera->transform->rotation.x; 
    //Disable fb
    reflectionFramebuffer->Disable();
    //____________________________________________
    
    //2. Render refraction
    //____________________________________________
    refractionFramebuffer->Enable();    
    ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
    ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);

    for(int i=0; i<scene->objects3D.size(); i++) {
        if(scene->objects3D[i] && scene->objects3D[i]->visible && scene->objects3D[i] != this) {
            MaterialComponent* material = (MaterialComponent*) scene->objects3D[i]->GetComponent("Material");
            if(material != nullptr) {
                int program = material->shader->programShaderObject;
                ogl->glUseProgram(program);
                GLuint loc = ogl->glGetUniformLocation(program, "clipPlane");
                ogl->glUniform4fv(loc, 1, glm::value_ptr(glm::vec4(0, -1, 0, this->transform->position.y)));
                scene->objects3D[i]->Render();
            }
        }
    }

    refractionFramebuffer->Disable();
    //____________________________________________
    ogl->glDisable(GL_CLIP_DISTANCE0); 

    ogl->glUseProgram(waterShader.programShaderObject);
    
    ogl->glActiveTexture(GL_TEXTURE0);
    ogl->glBindTexture(GL_TEXTURE_2D, reflectionFramebuffer->texture);
    ogl->glUniform1i(ogl->glGetUniformLocation(waterShader.programShaderObject, "reflectionTexture"), 0);

    ogl->glActiveTexture(GL_TEXTURE1);
    ogl->glBindTexture(GL_TEXTURE_2D, refractionFramebuffer->texture);
    ogl->glUniform1i(ogl->glGetUniformLocation(waterShader.programShaderObject, "refractionTexture"), 1);
    
    ogl->glActiveTexture(GL_TEXTURE2);
    ogl->glBindTexture(GL_TEXTURE_2D, dudvMap.glTex);
    ogl->glUniform1i(ogl->glGetUniformLocation(waterShader.programShaderObject, "dudvmap"), 2);
    
    ogl->glActiveTexture(GL_TEXTURE3);
    ogl->glBindTexture(GL_TEXTURE_2D, normalMap.glTex);
    ogl->glUniform1i(ogl->glGetUniformLocation(waterShader.programShaderObject, "normalMap"), 3);

    moveFactor += speed * scene->deltaTime;
    moveFactor = moveFactor >= 1 ? 0 : moveFactor;
    
    ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "moveFactor"), moveFactor);

    glm::vec3 camPos = scene->camera->transform->position;
    ogl->glUniform3fv(ogl->glGetUniformLocation(waterShader.programShaderObject, "cameraPosition"),1, glm::value_ptr(camPos));    

    quad->Render();
    ogl->glBindTexture(GL_TEXTURE_2D, 0);
}

void WaterTile_1::DepthRenderPass(LightComponent* light) {
}

void WaterTile_1::Destroy() {
    delete reflectionFramebuffer;
    delete quad;
}

void WaterTile_1::Recompute() {
}


}
}