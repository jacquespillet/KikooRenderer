#include "ParticleShader.hpp"

namespace KikooRenderer {

namespace CoreEngine {
Shader GetParticleShader() {
    Shader particleShader;

    particleShader.vertSrc= R"(
    //attribs
    #version 440
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal;
    layout(location = 2) in vec2 uv;
    layout(location = 3) in vec4 color;

    uniform mat4 modelViewProjectionMatrix;
    uniform vec2 texOffset1;
    uniform vec2 texOffset2;
    uniform vec2 texCoordInfo; //Number of rows, blend factor

    out float blendFactor;
    out vec2 fragmentUv;
    out vec2 fragmentUv1;
    out vec2 fragmentUv2;

    void main()
    {
        fragmentUv = uv;
        fragmentUv /= texCoordInfo.x; //Divide by number of rows
        
        fragmentUv1 = fragmentUv + texOffset1;
        fragmentUv2 = fragmentUv + texOffset2;
        blendFactor = texCoordInfo.y;
        

        vec4 finalPosition = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
        gl_Position = vec4(finalPosition.x, finalPosition.y, finalPosition.z, finalPosition.w);
    }
    )";

    particleShader.fragSrc = R"(
    #version 440
    layout(location = 0) out vec4 outputColor; 

    in vec2 fragmentUv;
    uniform sampler2D albedoTexture;
    uniform float green;

    in float blendFactor;
    in vec2 fragmentUv1;
    in vec2 fragmentUv2;    

    void main()
    {
        vec4 color1 = texture(albedoTexture, fragmentUv1);
        vec4 color2 = texture(albedoTexture, fragmentUv2);
        outputColor = mix(color1, color2, blendFactor);
    }
    )";
    particleShader.name = "particle Shader";
    particleShader.isLit = false;
    particleShader.isDepthPass = false;
    particleShader.Compile();

    return particleShader;
}
}
}