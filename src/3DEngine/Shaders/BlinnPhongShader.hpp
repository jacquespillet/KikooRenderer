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

    //Factors for each light component
    float ambientFactor = 0.6;
    float diffuseFactor = 0.6;
    float specularFactor = 0.6;
    int smoothness = 8;

    //Scales the normals of the normal map
    float normalMapInfluence = 1;
    
    //Normal map variables : Texture, name, should be reloaded at next frame
    Texture normalMap;
    std::string normalMapStr;
    bool shouldLoadNormal = false;
    
    //Specular map variables : Texture, name, should be reloaded at next frame
    Texture specularMap;
    std::string specularMapStr;
    bool shouldLoadSpecular = false;

    //Color of specular reflections
    glm::vec4 specularColor;

    bool reflectSkybox = false;

    BlinnPhongParams() {
        specularColor = glm::vec4(1);
    }

    virtual QJsonObject ToJSON() {
        QJsonObject json;

        json["ambientFactor"] =ambientFactor;
        json["diffuseFactor"] =diffuseFactor;
        json["specularFactor"] =specularFactor;
        json["smoothness"] =smoothness;

        json["normalMapInfluence"] =normalMapInfluence;
        
        json["normalMapStr"] = QString::fromStdString(normalMapStr);
        json["specularMapStr"] = QString::fromStdString(specularMapStr);
        
        QJsonObject specularColorJson;
        specularColorJson["r"] =specularColor.x; 
        specularColorJson["g"] =specularColor.y; 
        specularColorJson["b"] =specularColor.z; 
        specularColorJson["q"] =specularColor.w; 

        json["specularColor"] = specularColorJson;

        return json;
    }

    /*
    * Sets all the appropriate shader uniform variables
    * Called at each frame before rendering the object within a valid GL context
    */
    virtual void SetUniforms();
    
    virtual QLayout* GetLayout();
};

Shader GetBlinnPhongShader();
}
}