#include "UnlitShader.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{
Shader GetUnlitShader() {
    Shader unlitMeshShader;

    // UnlitMeshParams* params = new UnlitMeshParams();
    // params->shader = &unlitMeshShader;        
    
    // unlitMeshShader.SetParams(params);

    unlitMeshShader.vertSrc= R"(
    //attribs
    #version 440
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal;
    layout(location = 2) in vec2 uv;
    layout(location = 3) in vec4 color;
    //transforms
    uniform mat4 modelViewProjectionMatrix;
    uniform vec4 albedo; 
    uniform float materialInfluence;

    //outputs
    out vec4 fragmentColor;  
    out vec2 fragmentUv;
    //main
    void main()
    {
        //compute outputs
        fragmentColor = materialInfluence * albedo + (1.0 - materialInfluence) * color;
        vec4 finalPosition = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
        gl_Position = vec4(finalPosition.x, finalPosition.y, finalPosition.z, finalPosition.w);
        fragmentUv = uv;
    }
    )";

    unlitMeshShader.fragSrc = R"(
    //inputs
    #version 440
    in vec4 fragmentColor; 
    in vec2 fragmentUv;
    //uniforms
    uniform int hasAlbedoTex;
    uniform sampler2D albedoTexture;
    //output
    layout(location = 0) out vec4 outputColor; 
    //main
    void main()
    {
        outputColor = (hasAlbedoTex==1) ? texture(albedoTexture, fragmentUv) : fragmentColor;

    }
    )";
    return unlitMeshShader;
}
}
}