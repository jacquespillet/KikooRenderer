#pragma once

#include "Util/Common.h"
#include "../Scene.hpp"
#include "../Shader.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{
    
class Shader;
class ShaderParams : public QVBoxLayout {
Q_OBJECT
public:
    ShaderParams();

    virtual void SetUniforms()=0;
    virtual QLayout* GetLayout()=0;
    Shader* shader;
    Scene* scene;

signals: 
    void Modified();
};
}
}