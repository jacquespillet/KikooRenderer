#include "WaterTile_2.hpp"

#include "WaterTileShader_2.hpp"
#include "../../BaseObjects.hpp"
#include "../../Components/MaterialComponent.hpp"


namespace KikooRenderer {

namespace CoreEngine {

WaterTile_2::WaterTile_2(std::string name, Scene* scene) : Object3D(name, scene) {
    waterShader = GetWaterTile_2Shader();
    
    quad = GetQuad(scene, "particle", glm::dvec3(0, 0.2, 0), glm::dvec3(90, 0, 0), glm::dvec3(20), glm::dvec4(0.5, 0.5, 0.5, 1));
    quad->Start();
    quad->Enable();

    quadMaterial = (MaterialComponent*) quad->GetComponent("Material");
    quadMaterial->SetShader(&waterShader);

    // colorTexture = Texture("C:/Users/Jacques/Pictures/Textures/uv.png", GL_TEXTURE0);
    colorTexture = Texture("C:/Users/Jacques/Pictures/Textures/WaterTiles/2/water.png", GL_TEXTURE0);
    flowMap = Texture("C:/Users/Jacques/Pictures/Textures/WaterTiles/2/flowmap.png", GL_TEXTURE1, false);
    
}

void WaterTile_2::WindowResize(int w, int h) {
}


void WaterTile_2::Start() {
	started = true;
}

void WaterTile_2::Enable() {
	enabled = true;
}

void WaterTile_2::Update() {
    scene->triggerRefresh = true;
}

void WaterTile_2::Render(glm::mat4* overrideViewMatrixp) {
    GETGL
    ogl->glUseProgram(waterShader.programShaderObject);
        
    ogl->glActiveTexture(GL_TEXTURE0);
    ogl->glBindTexture(GL_TEXTURE_2D, colorTexture.glTex);
    ogl->glUniform1i(ogl->glGetUniformLocation(waterShader.programShaderObject, "colorTexture"), 0);

    ogl->glActiveTexture(GL_TEXTURE1);
    ogl->glBindTexture(GL_TEXTURE_2D, flowMap.glTex);
    ogl->glUniform1i(ogl->glGetUniformLocation(waterShader.programShaderObject, "flowMap"), 1);

    ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "time"), scene->elapsedTime);
    
    quad->Render();
}

void WaterTile_2::DepthRenderPass(LightComponent* light) {
}

void WaterTile_2::Destroy() {
    delete quad;
}

void WaterTile_2::Recompute() {
}


}
}