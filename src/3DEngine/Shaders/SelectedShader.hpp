#pragma once

#include "Util/Common.h"
#include "../Shader.hpp"
#include "ShaderParams.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{
class SelectedShaderParams : public ShaderParams {
public:
    SelectedShaderParams() : ShaderParams() {
    }

    float ambientFactor = 0.1;
    float diffuseFactor = 0.1;
    float specularFactor = 0.1;
    int smoothness = 2048;

    virtual void SetUniforms() {

    }

    virtual QJsonObject ToJSON() {
        QJsonObject json;
        return json;
    }

    virtual QLayout* GetLayout() {
        QVBoxLayout* shaderParamsLayout = new QVBoxLayout();
        return shaderParamsLayout;
    }
};

Shader GetSelectedShader();
}
}