#pragma once

#include "Util/Common.h"
#include "../Shader.hpp"
#include "ShaderParams.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{

class PBRParams : public ShaderParams {
public:
    PBRParams() {

    }

    float ambientFactor = 0.1;
    float diffuseFactor = 0.1;
    float specularFactor = 0.1;
    int smoothness = 2048;

    virtual void SetUniforms() {

    }

    virtual QJsonObject ToJSON() {
        QJsonObject json;
        json["roughness"] = smoothness;
        json["metalness"] = specularFactor;
        return json;
    }

    
    
    virtual QLayout* GetLayout() {
        QVBoxLayout* shaderParamsLayout = new QVBoxLayout();
        return shaderParamsLayout;
    }
};


Shader GetPBRShader();
}
}