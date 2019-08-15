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

    }
    
    virtual QLayout* GetLayout() {
        QVBoxLayout* shaderParamsLayout = new QVBoxLayout();
        return shaderParamsLayout;
    }
};

Shader GetBlinnPhongShader();
}
}