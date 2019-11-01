#include "LensFlare.hpp"

#include "../../BaseObjects.hpp"
#include "../../Components/MaterialComponent.hpp"
#include "../../Components/MeshFilterComponent.hpp"
#include "3DEngine/CameraScene.hpp"
#include "LensFlareShader.hpp"

namespace KikooRenderer {

namespace CoreEngine {

LensFlare::LensFlare(std::string name, Scene* scene) : Object3D(name, scene) {
    lensFlareShader = GetLensFlareShader();

    quad = GetQuad(scene, "flare", glm::vec3(-1, 0.01, -1), glm::vec3(0, 0, 0), glm::vec3(1), glm::vec4(0.5, 0.5, 0.5, 1));
    quad->Start();
    quad->Enable();


    quamaterial = quad->GetComponent<MaterialComponent>();
    quamaterial->SetShader(lensFlareShader);

    // textures.push_back(tex);
    textures.push_back(Texture("resources/Textures/LensFlare/tex4.png", GL_TEXTURE0));
    textures.push_back(Texture("resources/Textures/LensFlare/tex8.png", GL_TEXTURE0));
    textures.push_back(Texture("resources/Textures/LensFlare/tex4.png", GL_TEXTURE0));
    textures.push_back(Texture("resources/Textures/LensFlare/tex8.png", GL_TEXTURE0));

    sizes.push_back(glm::vec2(0.15));
    sizes.push_back(glm::vec2(0.035));
    sizes.push_back(glm::vec2(0.035));
    sizes.push_back(glm::vec2(0.15));



    spacing = 0.1;

    mesh = quad->GetComponent<MeshFilterComponent>();

    camera = scene->camera;
}

void LensFlare::WindowResize(int w, int h) {
}


void LensFlare::Start() {
	started = true;
}

void LensFlare::Enable() {
	enabled = true;
}

void LensFlare::Update() {
    scene->triggerRefresh = true;
}

std::vector<QWidget*> LensFlare::GetInspectorWidgets() {
    std::vector<QWidget*> res;
    return res;
}


void LensFlare::Render(glm::mat4* overrideViewMatrixp) {
 
}

void LensFlare::LateRender() {
   GETGL

   if(scene->lightObjects.size() > 0) {
        glm::mat4 mvpMatrix = camera->GetProjectionMatrix() * camera->GetViewMatrix() * scene->lightObjects[0]->transform->GetWorldModelMatrix();       
        
        glm::vec4 projectedLightPos(0, 0, 0, 1);
        projectedLightPos = mvpMatrix * projectedLightPos;
        if(projectedLightPos.w > 0) {
            projectedLightPos /= projectedLightPos.w;

            glm::vec2 screenSpaceLightPos = glm::vec2(projectedLightPos.x, projectedLightPos.y);
            glm::vec2 lightToCenter = -screenSpaceLightPos;

            for(int i=0; i<textures.size(); i++) {
                glm::vec2 direction = glm::normalize(lightToCenter);
                direction *= (i * spacing);

                glm::vec3 position = glm::vec3(screenSpaceLightPos, -0.01 * i) + glm::vec3(direction, 0);
                glm::vec3 size(sizes[i], 1);   

                ogl->glUseProgram(lensFlareShader.programShaderObject);
                ogl->glUniform3fv(ogl->glGetUniformLocation(lensFlareShader.programShaderObject, "screenPosition"), 1, glm::value_ptr(position));
                ogl->glUniform3fv(ogl->glGetUniformLocation(lensFlareShader.programShaderObject, "size"), 1, glm::value_ptr(size));

                ogl->glActiveTexture(GL_TEXTURE0);
                ogl->glBindTexture(GL_TEXTURE_2D, textures[i].glTex);
                ogl->glUniform1i(ogl->glGetUniformLocation(lensFlareShader.programShaderObject, "albedoTexture"), 0);    

                quad->Render();
            }    
        }
   }
   quamaterial->firstIter = false;
    
}

void LensFlare::DepthRenderPass(LightComponent* light) {
}

void LensFlare::Destroy() {
}

void LensFlare::Recompute() {
}


}
}