#pragma once

#include "Util/Common.h"
#include "../Shader.hpp"
#include "ShaderParams.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{
class AxesHelperParams : public ShaderParams {
public:
    AxesHelperParams() : ShaderParams() {
    }

    virtual void SetUniforms();

    virtual QLayout* GetLayout() {
        QVBoxLayout* shaderParamsLayout = new QVBoxLayout();
        return shaderParamsLayout;
    }

    virtual QJsonObject ToJSON() {
        QJsonObject json;
        json["type"] = "axesHelper";
        return json;
    }
};

Shader GetAxisHelperShader();
}
}