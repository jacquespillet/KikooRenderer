#include "WaterTile_2.hpp"

#include "WaterTileShader_2.hpp"
#include "DirectionalFlowShader.hpp"
#include "../../BaseObjects.hpp"
#include "../../Components/MaterialComponent.hpp"


namespace KikooRenderer {

namespace CoreEngine {

WaterTile_2::WaterTile_2(std::string name, Scene* scene) : Object3D(name, scene) {
    // waterShader = GetWaterTile_2Shader();
    if(tileType == TILE_TYPE::WATER) {
        waterShader = GetWaterTile_2Shader();
        colorTexture = Texture("C:/Users/Jacques/Pictures/Textures/WaterTiles/2/water.png", GL_TEXTURE0);
        flowMap = Texture("C:/Users/Jacques/Pictures/Textures/WaterTiles/2/flow-speed-noise.png", GL_TEXTURE1);
        normalMap = Texture("C:/Users/Jacques/Pictures/Textures/WaterTiles/2/water-derivative-height.png", GL_TEXTURE2, false);
    } else {
        waterShader = GetDirectionalFlowShader();
        colorTexture = Texture("C:/Users/Jacques/Pictures/Textures/WaterTiles/2/water.png", GL_TEXTURE0);
        flowMap = Texture("C:/Users/Jacques/Pictures/Textures/WaterTiles/2/flow-speed-noise.png", GL_TEXTURE1);
        normalMap = Texture("C:/Users/Jacques/Pictures/Textures/WaterTiles/2/ripples-derivative-height.png", GL_TEXTURE2, false);
    }
    
    
    quad = GetQuad(scene, "plane", glm::dvec3(0, 0 , 0), glm::dvec3(90, 0, 0), glm::dvec3(1), glm::dvec4(0.5, 0.5, 0.5, 1));
    quad->Start();
    quad->Enable();

    quadMaterial = (MaterialComponent*) quad->GetComponent("Material");
    quadMaterial->SetShader(&waterShader);

    
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
    
    if(colorTexture.loaded) {
        ogl->glActiveTexture(GL_TEXTURE0);
        ogl->glBindTexture(GL_TEXTURE_2D, colorTexture.glTex);
        ogl->glUniform1i(ogl->glGetUniformLocation(waterShader.programShaderObject, "colorTexture"), 0);
    }

    if(flowMap.loaded) {
        ogl->glActiveTexture(GL_TEXTURE1);
        ogl->glBindTexture(GL_TEXTURE_2D, flowMap.glTex);
        ogl->glUniform1i(ogl->glGetUniformLocation(waterShader.programShaderObject, "flowMap"), 1);
    }

    if(normalMap.loaded) {
        ogl->glActiveTexture(GL_TEXTURE2);
        ogl->glBindTexture(GL_TEXTURE_2D, normalMap.glTex);
        ogl->glUniform1i(ogl->glGetUniformLocation(waterShader.programShaderObject, "normalMap"), 2);
    }

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