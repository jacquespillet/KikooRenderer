#include "LensFlareShader.hpp"
#include "3DEngine/StandardShaders.hpp"

namespace KikooRenderer {

namespace CoreEngine {
Shader GetLensFlareShader() {
    Shader lensFlareShader;

    lensFlareShader.vertSrc= R"(
    //attribs
    #version 440
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal;
    layout(location = 2) in vec2 uv;
    layout(location = 3) in vec4 color;

    uniform mat4 modelViewProjectionMatrix;
    uniform vec3 screenPosition;
    uniform vec3 size;

    out vec2 fragmentUv;
    out float brightness;

    void main() {
        vec4 finalPosition = vec4(position.x, position.y, position.z, 1);
        finalPosition.xyz *= size;
        finalPosition.xyz += screenPosition;

        brightness = length(finalPosition.xy);
        fragmentUv = uv;
        gl_Position = finalPosition;
    }
    )";

    lensFlareShader.fragSrc = R"(
    #version 440
    layout(location = 0) out vec4 outputColor; 

    uniform sampler2D albedoTexture;

    in vec2 fragmentUv;
    in float brightness;
    void main()
    {
        outputColor =  texture(albedoTexture, fragmentUv);
        if(outputColor.a > 0.2) {
            outputColor.a = max(outputColor.a - brightness, 0.1);
        }
    }
    )";
    lensFlareShader.name = "water tile Shader 1";
    lensFlareShader.isLit = true;
    lensFlareShader.isDepthPass = false;
    lensFlareShader.SetId(SHADER_IDS::OTHER);
    lensFlareShader.Compile();
    lensFlareShader.shouldRecompile = false;

    return lensFlareShader;
}
}
}