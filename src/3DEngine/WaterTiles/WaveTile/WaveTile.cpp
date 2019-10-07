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

std::vector<QWidget*> WaveTile::GetInspectorWidgets() {
    std::vector<QWidget*> res;
    res.push_back(this->quad->transform->GetInspector());
    
    QGroupBox* mainGroupbox = new QGroupBox("Waves Inspector");
    QVBoxLayout* mainLayout = new QVBoxLayout();
    
    Vector4ArrayInspector* wavesInspector = new Vector4ArrayInspector("Waves : x, y, steepness, wavelength", waves, glm::vec4(1, 0, 0.75, 6)); 
    QObject::connect(wavesInspector, &Vector4ArrayInspector::Modified, [this](std::vector<glm::vec4> vectors) {
        waves = vectors;
    });    
    mainLayout->addWidget(wavesInspector);

    mainGroupbox->setLayout(mainLayout);
    res.push_back(mainGroupbox);

    return res;
}


void WaveTile::Render(glm::mat4* overrideViewMatrixp) {
    GETGL
    ogl->glUseProgram(waterShader.programShaderObject);
    ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "time"), scene->elapsedTime);
    
    glm::mat4 viewProjection = scene->camera->GetProjectionMatrix() * scene->camera->GetViewMatrix();
    int viewProjectionMatLoc = ogl->glGetUniformLocation(waterShader.programShaderObject, "viewProjectionMatrix"); 
    ogl->glUniformMatrix4fv(viewProjectionMatLoc, 1, false, glm::value_ptr(viewProjection));

    for(int i=0; i<waves.size(); i++) {
        std::string name = "waves[" + std::to_string(i) + "]";
        ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, name.c_str()), 1, glm::value_ptr(waves[i]));
    }

    // ogl->glUniform4fv(ogl->glGetUniformLocation(waterShader.programShaderObject, "waves[0]"), 1, glm::value_ptr(glm::vec4(1, 0, 0.75, 6)));
    
    ogl->glUniform1i(ogl->glGetUniformLocation(waterShader.programShaderObject, "numWaves"), waves.size());

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