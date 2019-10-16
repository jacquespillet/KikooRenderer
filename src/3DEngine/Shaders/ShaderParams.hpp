#pragma once

#include "Util/Common.h"

namespace KikooRenderer
{
namespace CoreEngine
{
    
class Shader;
class Scene;

class ShaderParams : public QVBoxLayout {
Q_OBJECT
public:
    ShaderParams();

    virtual void SetUniforms()=0;
    virtual QLayout* GetLayout()=0;
    Shader* shader;
    Scene* scene;

    virtual QJsonObject ToJSON()=0;

signals: 
    void Modified();
};
}
}