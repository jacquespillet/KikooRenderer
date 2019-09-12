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
    uniform int hasCubemap;

    //outputs
    out vec4 fragmentColor;  
    out vec2 fragmentUv;
    out vec3 cubeTexCoords;
    //main
    void main()
    {
        fragmentColor = materialInfluence * albedo + (1.0 - materialInfluence) * color;

        vec4 finalPosition = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);

        fragmentUv = uv;
        cubeTexCoords = position.xyz;

        gl_Position = vec4(finalPosition.x, finalPosition.y, finalPosition.z, finalPosition.w);
        if(hasCubemap > 0) gl_Position = gl_Position.xyww; 
    }
    )";

    unlitMeshShader.fragSrc = R"(
    //inputs
    #version 440
    in vec4 fragmentColor; 
    in vec2 fragmentUv;
    in vec3 cubeTexCoords;
    //uniforms
    uniform int hasAlbedoTex;
    uniform sampler2D albedoTexture;
    uniform samplerCube cubemapTexture;

    uniform int hasCubemap;
    //output
    layout(location = 0) out vec4 outputColor; 
    //main
    void main()
    {
        if(hasCubemap > 0) {
            outputColor = vec4(texture(cubemapTexture, cubeTexCoords).xyz, 1);
        } else {
            outputColor = (hasAlbedoTex==1) ? texture(albedoTexture, fragmentUv) : fragmentColor;
        }
    }
    )";
    return unlitMeshShader;
}
}
}