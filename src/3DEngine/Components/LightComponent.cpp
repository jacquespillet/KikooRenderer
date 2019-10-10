#include "LightComponent.hpp"
#include "TransformComponent.hpp"
#include "../Scene.hpp"

#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#define GLV QOpenGLFunctions_3_3_Core
#define GETGL GLV* ogl = QOpenGLContext::currentContext()->versionFunctions<GLV>(); if(ogl==NULL){std::cout << "could not get opengl context";}

namespace KikooRenderer {
namespace CoreEngine {
    
LightInspector::LightInspector(LightComponent* lightComponent) : ComponentInspector("Light", lightComponent) {
	this->lightComponent = lightComponent;
	Object3D* object = lightComponent->object3D;
	scene = object->scene;
	mainLayout = new QVBoxLayout();

    QHBoxLayout* lightTypeLayout = new QHBoxLayout();
	QComboBox* lightTypeList = new QComboBox();
	QLabel* lightTypeLabel = new QLabel("Light Type");
    
    lightTypeList->addItem("Directional");
    lightTypeList->addItem("Point");
    lightTypeList->addItem("Spot");
	
	lightTypeList->setCurrentIndex(lightComponent->type);
	
    lightTypeLayout->addWidget(lightTypeLabel);
	lightTypeLayout->addWidget(lightTypeList);
	
    mainLayout->addLayout(lightTypeLayout);

	connect(lightTypeList, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, [this,lightComponent](int index) {
        lightComponent->hasChanged = true;    
        lightComponent->SetType(index);
    });

    
	ColorPicker* lightColorPicker = new ColorPicker("LightColor", 255, 255, 255, 255);
	mainLayout->addWidget(lightColorPicker);
	connect(lightColorPicker, &ColorPicker::ColorPicked, this, [this, lightComponent](QColor color) {
        lightComponent->color = glm::vec4(color.red(), color.green(), color.blue(), color.alpha()) * 0.00392156;
        lightComponent->hasChanged = true;
		scene->triggerRefresh = true;
	});

    CustomSlider* intensitySlider = new CustomSlider(0.0f, 2.0, 0.01, "Intensity", 1.0);
    mainLayout->addLayout(intensitySlider);
    QObject::connect(intensitySlider, &CustomSlider::Modified, [this,lightComponent](double val) {
        lightComponent->intensity = val;
        lightComponent->hasChanged = true;
        scene->triggerRefresh = true;
    });


    CustomSlider* attenuationSlider = new CustomSlider(0.0, 1.0, 0.01, "Attenuation", 1.0);
    mainLayout->addLayout(attenuationSlider);
    QObject::connect(attenuationSlider, &CustomSlider::Modified, [this,lightComponent](double val) {
        lightComponent->attenuation.x = val;
        lightComponent->hasChanged = true;
        scene->triggerRefresh = true;
    });

    CustomSlider* biasSlider = new CustomSlider(0.0, 0.1, 0.0001, "Bias", 0.005);
    mainLayout->addLayout(biasSlider);
    QObject::connect(biasSlider, &CustomSlider::Modified, [this,lightComponent](double val) {
        lightComponent->bias = val;
        lightComponent->hasChanged = true;
        scene->triggerRefresh = true;
    });    

    QCheckBox* castShadowCheckbox = new QCheckBox("Cast Shadow");
    castShadowCheckbox->setChecked(lightComponent->castShadow);
    mainLayout->addWidget(castShadowCheckbox);
    QObject::connect(castShadowCheckbox, &QCheckBox::stateChanged, [this,lightComponent](int state) {
        lightComponent->castShadow = state > 0;
        scene->glWindow->makeCurrent();
        if(lightComponent->depthFBO != nullptr) lightComponent->depthFBO->Clear();
        if(lightComponent->depthCubeFBO != nullptr) lightComponent->depthCubeFBO->Clear();
        scene->glWindow->doneCurrent();
        lightComponent->hasChanged = true;
        scene->triggerRefresh = true;
    });

    //Directional specific
        //Variance
        //PCF
        //Poisson Disk Sampling

    //Point Specific
        //Variance
        //PCF

    //Perspective Specific
        //Variance
        //PCF
    
	setLayout(mainLayout);
}

LightComponent::LightComponent(Object3D* object, glm::vec4 color, glm::vec3 attenuation, int type) : Component("Light", object) {
    this->attenuation =  attenuation;
    this->color = color;   
    this->fov = 80;

    SetType(type);
}

void LightComponent::SetType(int type) {
    this->type = type;
    object3D->scene->glWindow->makeCurrent();
    if(type==0) {
        //Avant dernier arg TRUE pour debug, doit etre FALSE
        depthFBO = new Framebuffer(SHADOW_WIDTH, SHADOW_HEIGHT, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, false, true);
        lightProjection  = glm::orthoLH(-20.0, 20.0, -20.0, 20.0, 1.0, (double)farClip);

        depthPassShader.vertSrc= R"(
        #version 440
        layout(location = 0) in vec3 position;

        uniform mat4 modelViewProjectionMatrix;
        void main()
        {
            gl_Position = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
        }
        )";

        depthPassShader.fragSrc = R"(
        #version 440
        layout(location = 0) out vec4 outputColor; 
        void main()
        {   
            outputColor = vec4(1);
        }
        )";
        depthPassShader.name = "quad";
        depthPassShader.isLit = false;
        depthPassShader.isDepthPass = true;
        std::cout << "LightComponent:SetType:Compiling directional light depth Pass Shader" << std::endl; 
        depthPassShader.Compile();
        depthPassShader.shouldRecompile = false;
    } else if(type == 1) {
        depthCubeFBO = new CubeFramebuffer(1024, 1024, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, false, true);
        lightSpaceMatrices.resize(6);

        cubeDepthPassShader.vertSrc= R"(
        #version 330 core
        layout(location = 0) in vec3 position;

        uniform mat4 modelMatrix;
        void main()
        {
            gl_Position = modelMatrix * vec4(position.x, position.y, position.z, 1.0f);
        }
        )";

        cubeDepthPassShader.geometrySrc= R"(
        #version 330 core
        layout (triangles) in;
        layout (triangle_strip, max_vertices=18) out;

        uniform mat4 shadowMatrices[6];

        out vec4 FragPos; // FragPos from GS (output per emitvertex)

        void main()
        {
            for(int face = 0; face < 6; ++face)
            {
                gl_Layer = face; // built-in variable that specifies to which face we render.
                for(int i = 0; i < 3; ++i) // for each triangle's vertices
                {
                    FragPos = gl_in[i].gl_Position;
                    gl_Position = shadowMatrices[face] * FragPos;
                    EmitVertex();
                }    
                EndPrimitive();
            }
        }  
        )";


        cubeDepthPassShader.fragSrc = R"(
        #version 330 core
        in vec4 FragPos;

        uniform vec3 lightPos;
        uniform float farPlane;

        void main()
        {
            // get distance between fragment and light source
            float lightDistance = length(FragPos.xyz - lightPos);
            
            // map to [0;1] range by dividing by farPlane
            lightDistance = lightDistance / farPlane;
            
            // write this as modified depth
            gl_FragDepth = lightDistance;
        }
        )";
        cubeDepthPassShader.name = "quad";
        cubeDepthPassShader.isLit = false;
        cubeDepthPassShader.isDepthPass = true;
        std::cout << "LightComponent:SetType: Compiling pointlight Depth Pass Shader" << std::endl; 
        cubeDepthPassShader.Compile();
        cubeDepthPassShader.shouldRecompile = false;

    } else if(type==2) {
        //Avant dernier arg TRUE pour debug, doit etre FALSE
        depthFBO = new Framebuffer(1024, 1024, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, false, true);
        lightProjection  = glm::perspectiveLH(80.0f, 1.0f, 0.01f, farClip);
        // lightProjection  = glm::perspectiveLH((float)(DEGTORAD * fov), 1.0f, 0.01f, 100.0f);

        depthPassShader.vertSrc= R"(
        #version 440
        layout(location = 0) in vec3 position;

        uniform mat4 modelViewProjectionMatrix;
        void main()
        {
            gl_Position = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
        }
        )";

        depthPassShader.fragSrc = R"(
        #version 440
        layout(location = 0) out vec4 outputColor; 
        void main()
        {   
            outputColor = vec4(0);
        }
        )";
        depthPassShader.name = "quad";
        depthPassShader.isLit = false;
        depthPassShader.isDepthPass = true;
        std::cout << "LightComponent:SetType: Compiling spotlight Depth Pass Shader" << std::endl; 
        depthPassShader.Compile();
        depthPassShader.shouldRecompile = false;
    }
    object3D->scene->glWindow->doneCurrent();
    object3D->scene->triggerRefresh = true;
    
    hasChanged = true;
}

ComponentInspector* LightComponent::GetInspector() {
	lightInspector = new LightInspector(this);
	return lightInspector;
}

void LightComponent::RenderDepthMap() {
    GETGL
    if(castShadow) {
        ogl->glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        if(type==0) {
            depthFBO->Enable(); 
            ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
            ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);

            glm::mat4 model = object3D->transform->GetWorldRotationMatrix();
            glm::vec3 lightPos = -20.0 * glm::vec4(glm::column(model, 2));
            model = glm::translate(model, lightPos);
            // model = object3D->transform->GetWorldModelMatrix();
            viewMat = glm::inverse(model);
            lightSpaceMatrix = lightProjection * viewMat; 

            ogl->glCullFace(GL_FRONT);
            for(int i=0; i<object3D->scene->objects3D.size(); i++) {
                if(object3D->scene->objects3D[i] && object3D->scene->objects3D[i]->visible && object3D->scene->objects3D[i]->castShadow) {
                    MaterialComponent* material = object3D->scene->objects3D[i]->GetComponent<MaterialComponent>();
                    if(material != nullptr) {
                        material->shader.shouldRecompile = false;
                        
                        Shader tmpShader = material->shader;
                        ShaderParams* tmpParams = material->params;
                        
                        material->SetShader(depthPassShader);
                        
                        object3D->scene->objects3D[i]->DepthRenderPass(this); 
                        
                        material->SetShader(tmpShader);
                        material->params = tmpParams;
                    }
                }
            }
            ogl->glCullFace(GL_BACK);
            depthFBO->Disable();
        } else if(type == 1) {

            glm::vec3 lightPos = glm::vec3(object3D->transform->position);
            glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)1.0, 0.001f, farClip);

            lightSpaceMatrices[0] = (shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
            lightSpaceMatrices[1] = (shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
            lightSpaceMatrices[2] = (shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
            lightSpaceMatrices[3] = (shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
            lightSpaceMatrices[4] = (shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
            lightSpaceMatrices[5] = (shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));

            depthCubeFBO->Enable();
            ogl->glClear(GL_DEPTH_BUFFER_BIT);

            for(int i=0; i<object3D->scene->objects3D.size(); i++) {
                if(object3D->scene->objects3D[i] && object3D->scene->objects3D[i]->visible && object3D->scene->objects3D[i]->castShadow) {
                    MaterialComponent* material = object3D->scene->objects3D[i]->GetComponent<MaterialComponent>();
                    if(material != nullptr) {
                        material->shader.shouldRecompile = false;
                        Shader tmpShader = material->shader;

                        ShaderParams* tmpParams = material->params;
                        
                        material->SetShader(cubeDepthPassShader);
                        
                        object3D->scene->objects3D[i]->DepthRenderPass(this); 
                        
                        material->SetShader(tmpShader);
                        material->params = tmpParams;
                    }
                }
            }
            depthCubeFBO->Disable();
        } else  if(type==2) {
            depthFBO->Enable(); 
            ogl->glClearColor(0.2, 0.2, 0.2, 1.0);
            ogl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_STENCIL_BUFFER_BIT);

            glm::mat4 model = object3D->transform->GetWorldModelMatrix();
            viewMat = glm::inverse(model);
            lightSpaceMatrix = lightProjection * viewMat;

            for(int i=0; i<object3D->scene->objects3D.size(); i++) {
                if(object3D->scene->objects3D[i] && object3D->scene->objects3D[i]->visible && object3D->scene->objects3D[i]->castShadow) {
                    MaterialComponent* material = object3D->scene->objects3D[i]->GetComponent<MaterialComponent>();
                    if(material != nullptr) {
                        material->shader.shouldRecompile = false;
                        Shader tmpShader = material->shader;
                        ShaderParams* tmpParams = material->params;
                        material->SetShader(depthPassShader);
                        object3D->scene->objects3D[i]->DepthRenderPass(this); 
                        material->SetShader(tmpShader);
                        material->params = tmpParams;
                    }
                }
            }

            depthFBO->Disable();
        }
    }
}

void LightComponent::SetShaderUniforms() {}

void LightComponent::OnStart(){}
void LightComponent::OnEnable(){}
void LightComponent::OnUpdate(){}
void LightComponent::OnRender(){} 
void LightComponent::OnDestroy(){} 
void LightComponent::Recompute(){}


void LightComponent::FromJSON(QJsonObject json, Object3D* obj) {
    int type = json["type"].toInt();
    
    glm::vec3 attenuation;
    QJsonObject attenuationJson = json["attenuation"].toObject();
    attenuation.x = attenuationJson["x"].toDouble();
    attenuation.y = attenuationJson["y"].toDouble();
    attenuation.z = attenuationJson["z"].toDouble();

    glm::vec4 color;
    QJsonObject colorJson = json["color"].toObject();
    color.x = colorJson["r"].toDouble();
    color.y = colorJson["g"].toDouble();
    color.z = colorJson["b"].toDouble();
    color.w = colorJson["a"].toDouble();

    double fov = colorJson["fov"].toDouble();

    LightComponent* light = new LightComponent(obj, color, attenuation, type);

    obj->AddComponent(light);            
}

}
}