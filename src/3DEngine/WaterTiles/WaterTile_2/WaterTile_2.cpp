#include "WaterTile_2.hpp"

#include "WaterTileShader_2.hpp"
#include "DirectionalFlowShader.hpp"
#include "../../BaseObjects.hpp"
#include "../../Components/MaterialComponent.hpp"


namespace KikooRenderer {

namespace CoreEngine {

WaterTile_2::WaterTile_2(std::string name, Scene* scene, TILE_TYPE type) : Object3D(name, scene) {
    tileType = type;
    if(tileType == TILE_TYPE::WATER) {
        waterShader = GetWaterTile_2Shader();
        colorTexture = Texture("resources/Textures/WaterTiles/2/water.png", GL_TEXTURE0);
        flowMap = Texture("resources/Textures/WaterTiles/2/flow-speed-noise.png", GL_TEXTURE1);
        normalMap = Texture("resources/Textures/WaterTiles/2/water-derivative-height.png", GL_TEXTURE2, false);
    } else {
        waterShader = GetDirectionalFlowShader();
        colorTexture = Texture("resources/Textures/WaterTiles/2/water.png", GL_TEXTURE0);
        flowMap = Texture("resources/Textures/WaterTiles/2/flow-speed-noise.png", GL_TEXTURE1);
        normalMap = Texture("resources/Textures/WaterTiles/2/ripples-derivative-height.png", GL_TEXTURE2, false);
    }
    
    
    quad = GetQuad(scene, "plane", glm::vec3(0, 0 , 0), glm::vec3(90, 0, 0), glm::vec3(1), glm::vec4(0.5, 0.5, 0.5, 1));
    quad->Start();
    quad->Enable();

    quamaterial = quad->GetComponent<MaterialComponent>();
    quamaterial->SetShader(waterShader);
}

std::vector<QWidget*> WaterTile_2::GetInspectorWidgets() {
    std::vector<QWidget*> res;
    res.push_back(this->transform->GetInspector());
    
    if(tileType == TILE_TYPE::DIRECTIONAL) {
        QGroupBox* mainGroupbox = new QGroupBox("Directional flow");
        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainGroupbox->setLayout(mainLayout);

        CustomSlider* tilingSlider = new CustomSlider(0.1, 10, 0.1, "Tiling", tiling);
        mainLayout->addLayout(tilingSlider);
        QObject::connect( tilingSlider, &CustomSlider::Modified, [this](double val) {
            tiling = val;
        });

        
        CustomSlider* speedSlider = new CustomSlider(0, 20, 0.1, "Speed", speed);
        mainLayout->addLayout(speedSlider);
        QObject::connect( speedSlider, &CustomSlider::Modified, [this](double val) {
            speed = val;
        });

        CustomSlider* strengthSlider = new CustomSlider(0, 2, 0.11, "strength", strength);
        mainLayout->addLayout(strengthSlider);
        QObject::connect( strengthSlider, &CustomSlider::Modified, [this](double val) {
            strength = val;
        });

        CustomSlider* constantHeightScaleSlider = new CustomSlider(0, 10, 0.1, "constant Height Scale", constantHeightScale);
        mainLayout->addLayout(constantHeightScaleSlider);
        QObject::connect( constantHeightScaleSlider, &CustomSlider::Modified, [this](double val) {
            constantHeightScale = val;
        });

        
        CustomSlider* modulatedHeightScaleSlider = new CustomSlider(0, 20, 0.1, "modeulated Height Scale", modulatedHeightScale);
        mainLayout->addLayout(modulatedHeightScaleSlider);
        QObject::connect( modulatedHeightScaleSlider, &CustomSlider::Modified, [this](double val) {
            modulatedHeightScale = val;
        });
        
        CustomSlider* gridResolutionSlider = new CustomSlider(0, 100, 2, "grid Resolution", gridResolution);
        mainLayout->addLayout(gridResolutionSlider);
        QObject::connect( gridResolutionSlider, &CustomSlider::Modified, [this](double val) {
            gridResolution = val;
        });

        CustomSlider* modulatedTilingSlider = new CustomSlider(0, 100, 2, "modulated Tiling", modulatedTiling);
        mainLayout->addLayout(modulatedTilingSlider);
        QObject::connect( modulatedTilingSlider, &CustomSlider::Modified, [this](double val) {
            modulatedTiling = val;
        });    

        res.push_back(mainGroupbox);
    } else {        
        QGroupBox* mainGroupbox = new QGroupBox("Directional flow");
        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainGroupbox->setLayout(mainLayout);

        CustomSlider* distortionTilingSlider = new CustomSlider(0, 100, 1, "Tiling", distortionTiling);
        mainLayout->addLayout(distortionTilingSlider);
        QObject::connect( distortionTilingSlider, &CustomSlider::Modified, [this](double val) {
            distortionTiling = val;
        });    
        
        CustomSlider* distortionSpeedSlider = new CustomSlider(0, 10, 0.01, "Speed", distortionSpeed);
        mainLayout->addLayout(distortionSpeedSlider);
        QObject::connect( distortionSpeedSlider, &CustomSlider::Modified, [this](double val) {
            distortionSpeed = val;
        });    

        CustomSlider* distortionStrengthSlider = new CustomSlider(0, 2, 0.01, "Strength", distortionStrength);
        mainLayout->addLayout(distortionStrengthSlider);
        QObject::connect( distortionStrengthSlider, &CustomSlider::Modified, [this](double val) {
            distortionStrength = val;
        });   

        CustomSlider* flowOffsetSlider = new CustomSlider(0, 10, 0.01, "Offset", flowOffset);
        mainLayout->addLayout(flowOffsetSlider);
        QObject::connect( flowOffsetSlider, &CustomSlider::Modified, [this](double val) {
            flowOffset = val;
        });   
        
        CustomSlider* distortionConstantHeightScaleSlider = new CustomSlider(0, 10, 0.01, "Constant Height", distortionConstantHeightScale);
        mainLayout->addLayout(distortionConstantHeightScaleSlider);
        QObject::connect( distortionConstantHeightScaleSlider, &CustomSlider::Modified, [this](double val) {
            distortionConstantHeightScale = val;
        });   
        
        CustomSlider* distortionModulatedHeightScaleSlider = new CustomSlider(0, 10, 0.01, "Modulated Height", distortionModulatedHeightScale);
        mainLayout->addLayout(distortionModulatedHeightScaleSlider);
        QObject::connect( distortionModulatedHeightScaleSlider, &CustomSlider::Modified, [this](double val) {
            distortionModulatedHeightScale = val;
        });   
        res.push_back(mainGroupbox);
    }
    return res;
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

    if(tileType == TILE_TYPE::DIRECTIONAL) {
        ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "tiling"), tiling);
        ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "speed"), speed);
        ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "strength"), strength);
        ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "constantHeightScale"), constantHeightScale);
        ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "modulatedHeightScale"), modulatedHeightScale);
        ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "gridResolution"), gridResolution);
        ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "modulatedTiling"), modulatedTiling);
        ogl->glUniform1i(ogl->glGetUniformLocation(waterShader.programShaderObject, "dualGrid"), dualGrid);
    } else {
        ogl->glUniform2fv(ogl->glGetUniformLocation(waterShader.programShaderObject, "uvJump"),1, glm::value_ptr(uvJump));
        ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "tiling"), distortionTiling );
        ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "speed"), distortionSpeed );
        ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "strength"), distortionStrength );
        ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "flowOffset"), flowOffset );
        ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "constantHeightScale"), distortionConstantHeightScale );
        ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "modulatedHeightScale"), distortionModulatedHeightScale );

    }
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