#include "WaterTile_1.hpp"

#include "WaterTileShader_1.hpp"
#include "../../BaseObjects.hpp"
#include "../../Components/MaterialComponent.hpp"


namespace KikooRenderer {

namespace CoreEngine {

WaterTile_1::WaterTile_1(std::string name, Scene* scene) : Object3D(name, scene) {
    waterShader = GetWaterTile_1Shader();
    
    quad = GetQuad(scene, "waterTile", glm::vec3(0, 0.2, 0), glm::vec3(90, 0, 0), glm::vec3(2), glm::vec4(0.5, 0.5, 0.5, 1));
    quad->Start();
    quad->Enable();

    quamaterial = quad->GetComponent<MaterialComponent>();
    quamaterial->SetShader(waterShader);
    quad->transform = this->transform;
    transform->rotation.x = 90;

    reflectionFramebuffer = new Framebuffer(scene->windowWidth,  scene->windowHeight,  GL_RGBA16F, GL_RGBA, GL_FLOAT, true, true, false);
    refractionFramebuffer = new Framebuffer(scene->windowWidth,  scene->windowHeight,  GL_RGBA16F, GL_RGBA, GL_FLOAT, true, true, false);

    dudvMap = Texture("resources/Textures/WaterTiles/1/waterDUDV.png", GL_TEXTURE2);
    normalMap = Texture("resources/Textures/WaterTiles/1/matchingNormalMap.png", GL_TEXTURE3);
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
    scene->triggerRefresh = true;
}

std::vector<QWidget*> WaterTile_1::GetInspectorWidgets() {
    std::vector<QWidget*> res;
    res.push_back(this->transform->GetInspector());
    
    QGroupBox* mainGroupbox = new QGroupBox("Simple DUDV flow");
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainGroupbox->setLayout(mainLayout);

    CustomSlider* waveStrengthSlider = new CustomSlider(0, 1, 0.01, "Wave Strength", waveStrength);
    mainLayout->addLayout(waveStrengthSlider);
    QObject::connect( waveStrengthSlider, &CustomSlider::Modified, [this](double val) {
        waveStrength = val;
    });

    
    CustomSlider* reflectivitySlider = new CustomSlider(0, 512, 2, "Reflectivity", reflectivity);
    mainLayout->addLayout(reflectivitySlider);
    QObject::connect( reflectivitySlider, &CustomSlider::Modified, [this](double val) {
        reflectivity = val;
    });

    CustomSlider* speedSlider = new CustomSlider(0, 1, 0.01, "Speed", speed);
    mainLayout->addLayout(speedSlider);
    QObject::connect( speedSlider, &CustomSlider::Modified, [this](double val) {
        speed = val;
    });

    
    CustomSlider* bluenessSlider = new CustomSlider(0, 1, 0.01, "blueness", blueness);
    mainLayout->addLayout(bluenessSlider);
    QObject::connect( bluenessSlider, &CustomSlider::Modified, [this](double val) {
        blueness = val;
    });

    res.push_back(mainGroupbox);


    return res;
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
            MaterialComponent* material = scene->objects3D[i]->GetComponent<MaterialComponent>();
            if(material != nullptr) {
                int program = material->shader.programShaderObject;
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
    // ____________________________________________
    
    // 2. Render refraction
    // ____________________________________________
    refractionFramebuffer->Enable();    
    ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
    ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);

    for(int i=0; i<scene->objects3D.size(); i++) {
        if(scene->objects3D[i] && scene->objects3D[i]->visible && scene->objects3D[i] != this) {
            MaterialComponent* material = scene->objects3D[i]->GetComponent<MaterialComponent>();
            if(material != nullptr) {
                int program = material->shader.programShaderObject;
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
    
    ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "waveStrength"), waveStrength);
    
    ogl->glUniform1i(ogl->glGetUniformLocation(waterShader.programShaderObject, "reflectivity"), reflectivity);

    ogl->glUniform1f(ogl->glGetUniformLocation(waterShader.programShaderObject, "blueness"), blueness);

    quad->Render();
    ogl->glBindTexture(GL_TEXTURE_2D, 0);
}

void WaterTile_1::DepthRenderPass(LightComponent* light) {
}

void WaterTile_1::Destroy() {
    delete reflectionFramebuffer;
    delete refractionFramebuffer;
    delete quad;
}

void WaterTile_1::Recompute() {
}


}
}