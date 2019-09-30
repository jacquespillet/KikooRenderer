#include "WaveTile.hpp"
#include "WaveTileShader.hpp"

#include "../../BaseObjects.hpp"
#include "../../Components/MaterialComponent.hpp"
#include "../../Components/MeshFilterComponent.hpp"


namespace KikooRenderer {

namespace CoreEngine {

WaveTile::WaveTile(std::string name, Scene* scene) : Object3D(name, scene) {
    waterShader = GetWaveTileShader();
    
    quad = GetTerrain(scene, "particle", glm::vec3(-1, 0.01, -1), glm::vec3(0, 0, 0), glm::vec3(1), glm::vec4(0.5, 0.5, 0.5, 1), 10, 10, 100, 100);
    quad->Start();
    quad->Enable();


    quamaterial = quad->GetComponent<MaterialComponent>();
    quamaterial->SetShader(waterShader);

    mesh = quad->GetComponent<MeshFilterComponent>();
}

void WaveTile::WindowResize(int w, int h) {
}


void WaveTile::Start() {
	started = true;
}

void WaveTile::Enable() {
	enabled = true;
}

void WaveTile::Update() {
    scene->triggerRefresh = true;
}

void WaveTile::Render(glm::mat4* overrideViewMatrixp) {
    GETGL
    ogl->glUseProgram(waterShader.programShaderObject);
    ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "time"), scene->elapsedTime);
    
    glm::mat4 viewProjection = scene->camera->GetProjectionMatrix() * scene->camera->GetViewMatrix();
    int viewProjectionMatLoc = ogl->glGetUniformLocation(waterShader.programShaderObject, "viewProjectionMatrix"); 
    ogl->glUniformMatrix4fv(viewProjectionMatLoc, 1, false, glm::value_ptr(viewProjection));


    quad->Render();
}

void WaveTile::DepthRenderPass(LightComponent* light) {
}

void WaveTile::Destroy() {
}

void WaveTile::Recompute() {
}


}
}