#include "LensFlare.hpp"

#include "../../BaseObjects.hpp"
#include "../../Components/MaterialComponent.hpp"
#include "../../Components/MeshFilterComponent.hpp"


namespace KikooRenderer {

namespace CoreEngine {

LensFlare::LensFlare(std::string name, Scene* scene) : Object3D(name, scene) {
    
    quad = GetQuad(scene, "particle", glm::vec3(-1, 0.01, -1), glm::vec3(0, 0, 0), glm::vec3(1), glm::vec4(0.5, 0.5, 0.5, 1));
    quad->Start();
    quad->Enable();


    quamaterial = quad->GetComponent<MaterialComponent>();
    
    mesh = quad->GetComponent<MeshFilterComponent>();
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
    GETGL
    // ogl->glUseProgram(waterShader.programShaderObject);
    // ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "time"), scene->elapsedTime);
    
    // glm::mat4 viewProjection = scene->camera->GetProjectionMatrix() * scene->camera->GetViewMatrix();
    // int viewProjectionMatLoc = ogl->glGetUniformLocation(waterShader.programShaderObject, "viewProjectionMatrix"); 
    // ogl->glUniformMatrix4fv(viewProjectionMatLoc, 1, false, glm::value_ptr(viewProjection));

    // for(int i=0; i<waves.size(); i++) {
    //     std::string name = "waves[" + std::to_string(i) + "]";
    //     ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, name.c_str()), 1, glm::value_ptr(waves[i]));
    // }

    // // ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, "waves[0]"), 1, glm::value_ptr(glm::vec4(1, 0, 0.75, 6)));
    
    // ogl->glUniform1i(ogl->glGetUniformLocation(waterShader.programShaderObject, "numWaves"), waves.size());

    // if(quamaterial->firstIter) quamaterial->firstIter = false;
    quad->Render();
}

void LensFlare::DepthRenderPass(LightComponent* light) {
}

void LensFlare::Destroy() {
}

void LensFlare::Recompute() {
}


}
}