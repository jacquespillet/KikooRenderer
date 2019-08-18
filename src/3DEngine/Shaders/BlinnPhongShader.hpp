#pragma once

#include "Util/Common.h"
#include "../Shader.hpp"
#include "ShaderParams.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{
class BlinnPhongParams : public ShaderParams {
public:
    BlinnPhongParams() {

    }

    float ambientFactor = 0.6;
    float diffuseFactor = 0.6;
    float specularFactor = 0.6;
    int smoothness = 8;

    float normalMapInfluence = 1;
    
    Texture normalMap;
    std::string normalMapStr;
    bool shouldLoadNormal = false;
    
    Texture specularMap;
    std::string specularMapStr;
    bool shouldLoadSpecular = false;

    virtual void SetUniforms() {
        GETGL

        if (shouldLoadNormal ) {
            normalMap = KikooRenderer::CoreEngine::Texture(normalMapStr, GL_TEXTURE2);
            shouldLoadNormal = false;
        }

        if (shouldLoadSpecular ) {
            specularMap = KikooRenderer::CoreEngine::Texture(specularMapStr, GL_TEXTURE1);
            shouldLoadSpecular = false;
        }
        
        if(normalMap.loaded) {
            normalMap.Use();
            int texLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "normalTexture"); 
            ogl->glUniform1i(texLocation, 2); 
            
            int hasNormalTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasNormalTex"); 
            ogl->glUniform1i(hasNormalTexLocation, 1);

            int normalMapInfluenceLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "normalMapInfluence");
            ogl->glUniform1f(normalMapInfluenceLocation, normalMapInfluence);
        }  else {
            int hasNormalTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasNormalTex"); 
            ogl->glUniform1i(hasNormalTexLocation, 0);
        } 

        if(specularMap.loaded) {
            specularMap.Use();
            int texLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "specularTexture"); 
            ogl->glUniform1i(texLocation, 1); 

            int hasSpecularTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasSpecularTex"); 
            ogl->glUniform1i(hasSpecularTexLocation, 1);
        } else {
            int hasSpecularTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasSpecularTex"); 
            ogl->glUniform1i(hasSpecularTexLocation, 0);
        }

        int ambientFactorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "ambientFactor");
        ogl->glUniform1f(ambientFactorLocation, ambientFactor);

        int diffuseFactorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "diffuseFactor");
        ogl->glUniform1f(diffuseFactorLocation, diffuseFactor);

        int specularFactorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "specularFactor");
        ogl->glUniform1f(specularFactorLocation, specularFactor);

        int smoothnessLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "smoothness");
        ogl->glUniform1f(smoothnessLocation, smoothness);
    }
    
    virtual QLayout* GetLayout() {
        QVBoxLayout* shaderParamsLayout = new QVBoxLayout();

        // Normal Map
        FilePicker* normalMapPicker = new FilePicker("Normal Map", normalMapStr);
        shaderParamsLayout->addWidget(normalMapPicker);
        connect(normalMapPicker, &FilePicker::FileModified, this, [this](QString string) {
            normalMapStr = string.toStdString();
            shouldLoadNormal = true;
            scene->triggerRefresh = true;
        });

        CustomSlider* normalMapInfluenceSlider = new CustomSlider(0.0f, 3.0f, 0.01, "Normal Map Influence", normalMapInfluence);
        shaderParamsLayout->addLayout(normalMapInfluenceSlider);
        QObject::connect(normalMapInfluenceSlider, &CustomSlider::Modified, [this](double val) {
            normalMapInfluence = val;
            scene->triggerRefresh = true;
        });

        //Specular Map
        FilePicker* specularPicker = new FilePicker("Specular Map", specularMapStr);
        shaderParamsLayout->addWidget(specularPicker);
        connect(specularPicker, &FilePicker::FileModified, this, [this](QString string) {
            specularMapStr = string.toStdString();
            shouldLoadSpecular = true;
            scene->triggerRefresh = true;
        });

        CustomSlider* ambientSlider = new CustomSlider(0, 1, 0.01, "Ambient Factor", ambientFactor);
        shaderParamsLayout->addLayout(ambientSlider);
        QObject::connect( ambientSlider, &CustomSlider::Modified, [this](double val) {
            ambientFactor = val;
            scene->triggerRefresh = true;
        });
    
        CustomSlider* diffuseSlider = new CustomSlider(0, 1, 0.01, "Diffuse Factor", diffuseFactor);
        shaderParamsLayout->addLayout(diffuseSlider);
        QObject::connect(diffuseSlider, &CustomSlider::Modified, [this](double val) {
            diffuseFactor = val;
            scene->triggerRefresh = true;
        });

        CustomSlider* specularSlider = new CustomSlider(0, 1, 0.01, "Specular Factor", specularFactor);
        shaderParamsLayout->addLayout(specularSlider);
        QObject::connect(specularSlider, &CustomSlider::Modified, [this](double val) {
            specularFactor = val;
            scene->triggerRefresh = true;
        });

        CustomSlider* smoothnessSlider = new CustomSlider(0, 128, 1, "Smoothness", smoothness);
        shaderParamsLayout->addLayout(smoothnessSlider);
        QObject::connect(smoothnessSlider, &CustomSlider::Modified, [this](double val) {
            smoothness = val;
            scene->triggerRefresh = true;
        });

        return shaderParamsLayout;
    }
};

Shader GetBlinnPhongShader();
}
}