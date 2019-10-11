#include "AxisHelperShader.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{
void AxesHelperParams::SetUniforms() {
    GETGL
    
    int viewRotationMatrix = ogl->glGetUniformLocation(this->shader->programShaderObject, "viewRotationMatrix"); 
    ogl->glUniformMatrix4fv(viewRotationMatrix, 1, false, glm::value_ptr(glm::inverse(scene->camera->transform->GetWorldRotationMatrix())));    
    
    int projectionMatrix = ogl->glGetUniformLocation(this->shader->programShaderObject, "projectionMatrix"); 
    ogl->glUniformMatrix4fv(projectionMatrix, 1, false, glm::value_ptr(scene->camera->GetProjectionMatrix()));
}

Shader GetAxisHelperShader() {
    Shader axisHelperShader;

    std::ifstream t("Shaders/Other/AxisHelper.vert");
    std::stringstream vertBuffer;
    vertBuffer << t.rdbuf();
    axisHelperShader.vertSrc= vertBuffer.str();

    
    t = std::ifstream ("Shaders/Other/AxisHelper.frag");
    std::stringstream fragBuffer;
    fragBuffer << t.rdbuf();
    axisHelperShader.fragSrc= fragBuffer.str();
    axisHelperShader.isLit = false;
    axisHelperShader.isDepthPass = false;
    axisHelperShader.SetId(5);

    return axisHelperShader;
}
}
}