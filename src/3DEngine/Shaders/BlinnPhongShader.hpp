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

    BlinnPhongParams() {
        specularColor = glm::vec4(1);
    }

    /*
    * Sets all the appropriate shader uniform variables
    * Called at each frame before rendering the object within a valid GL context
    */
    virtual void SetUniforms() {
        GETGL

        /*
        * When textures changed in the form, load them for the incoming frame
        */
        if (shouldLoadNormal ) {
            normalMap = KikooRenderer::CoreEngine::Texture(normalMapStr, GL_TEXTURE2);
            shouldLoadNormal = false;
        }

        if (shouldLoadSpecular ) {
            specularMap = KikooRenderer::CoreEngine::Texture(specularMapStr, GL_TEXTURE1);
            shouldLoadSpecular = false;
        }
        
        /*
        Set variables for normal mapping : 
            * Texture variable
            * bool hasNormalTex
            * Influence of normals of the map on the actual normal
        */
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

        /*
        Set variables for Specular mapping : 
            * Texture variable
            * bool hasSpecularTex
        */
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

        /*
        * Set all factors for computing blinn phong shading
        */
		int specularColorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "specularColor"); 
		ogl->glUniform4fv(specularColorLocation, 1, glm::value_ptr(specularColor));

        int ambientFactorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "ambientFactor");
        ogl->glUniform1f(ambientFactorLocation, ambientFactor);

        int diffuseFactorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "diffuseFactor");
        ogl->glUniform1f(diffuseFactorLocation, diffuseFactor);

        int specularFactorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "specularFactor");
        ogl->glUniform1f(specularFactorLocation, specularFactor);

        int smoothnessLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "smoothness");
        ogl->glUniform1f(smoothnessLocation, smoothness);
    }
    
    virtual QLayout* GetLayout();
};

Shader GetBlinnPhongShader();
}
}