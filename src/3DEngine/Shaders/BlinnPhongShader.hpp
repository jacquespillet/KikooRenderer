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

    float ambientFactor = 0.1;
    float diffuseFactor = 0.1;
    float specularFactor = 0.1;
    int smoothness = 2048;

    virtual void SetUniforms() {
        GETGL

        // std::cout << "INSIDE " << this->shader << "  " <<  this->shader->programShaderObject << std::endl;
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
        std::cout << ambientFactor << std::endl;

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

        CustomSlider* smoothnessSlider = new CustomSlider(0, 2048, 10, "Smoothness", smoothness);
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