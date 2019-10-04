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

    ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, "waves[0]"), 1, glm::value_ptr(glm::vec4(1, 0, 0.75, 6)));
    ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, "waves[1]"), 1, glm::value_ptr(glm::vec4(1, 1, 0.75, 4)));
    ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, "waves[2]"), 1, glm::value_ptr(glm::vec4(1, 1, 0.5, 2)));
    ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, "waves[3]"), 1, glm::value_ptr(glm::vec4(-1, 0, 0.35, 1.8)));
    ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, "waves[4]"), 1, glm::value_ptr(glm::vec4(1, -1, 0.25, 1.5)));
    ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, "waves[5]"), 1, glm::value_ptr(glm::vec4(-1, 1, 0.20, 1.3)));
    ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, "waves[6]"), 1, glm::value_ptr(glm::vec4(0, -1, 0.15, 1)));
    ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, "waves[7]"), 1, glm::value_ptr(glm::vec4(-1, -1, 0.1, 0.8)));
    ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, "waves[8]"), 1, glm::value_ptr(glm::vec4(-1, 0, 0.05, 0.5)));
    ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, "waves[9]"), 1, glm::value_ptr(glm::vec4(1, -1, 0.01, 0.23)));
    ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, "waves[10]"), 1, glm::value_ptr(glm::vec4(-1, 1, 0.01, 0.1)));
    ogl->glUniform1i(ogl->glGetUniformLocation(waterShader.programShaderObject, "numWaves"), 11);


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