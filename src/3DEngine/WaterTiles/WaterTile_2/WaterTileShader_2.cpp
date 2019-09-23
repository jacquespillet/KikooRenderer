#include "waterTileShader_2.hpp"

namespace KikooRenderer {

namespace CoreEngine {
Shader GetWaterTile_2Shader() {
    Shader waterTileShader;

    waterTileShader.vertSrc= R"(
    //attribs
    #version 440
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal;
    layout(location = 2) in vec2 uv;
    layout(location = 3) in vec4 color;

    uniform mat4 modelViewProjectionMatrix;
    uniform mat4 modelMatrix;

    out vec2 colorUv;
    out vec2 fragUv;

    void main() {
        colorUv = (uv);
        fragUv = uv;
        
        vec4 clipSpace =  modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
        gl_Position = clipSpace;        
    }
    )";

    waterTileShader.fragSrc = R"(
    #version 440
    layout(location = 0) out vec4 outputColor; 

    struct LightSource
    {
        int type;
        vec3 position;
        vec3 attenuation;
        vec3 direction;
        vec4 color;
        sampler2D depthMap;
        mat4 lightSpaceMatrix;
        samplerCube depthCubeMap;
        float farPlane;
    };   

    uniform LightSource lights[4];
    uniform int numLights; 
    uniform sampler2D colorTexture;
    uniform sampler2D flowMap;
    uniform sampler2D normalMap;
    uniform float time;
    
    in vec2 colorUv;
    in vec2 fragUv;

    const vec2 uvJump = vec2(0.0, 0.0);
    const float tiling = 3.0;
    const float speed = 0.5;
    const float strength = 0.1;
    const float flowOffset = 0;
    
    vec3 flowUVW(vec2 uv, vec2 flowVector,vec2 jump, float _time, bool flowB, float tiling) {
        float phaseOffset = flowB ? 0.5 : 0;
        
        float progress = fract(_time+ phaseOffset);

        vec3 uvw;
        uvw.xy = uv - flowVector * progress;
        uvw.xy *= tiling;
        uvw.xy += phaseOffset; 

        uvw.xy += (_time - progress) * jump;

        uvw.z = 1 - abs(1 - 2 * progress);

        return uvw;
    }
 
    void main(){
        vec2 flowVector = texture(flowMap, fragUv).xy * 2.0 - 1.0;
        flowVector *= strength;
        
        float noise = texture(flowMap, fragUv).a;
        float _time = time * speed + noise;    

        vec3 uvwA = flowUVW(fragUv, flowVector, uvJump, _time, false, tiling);
        vec3 uvwB = flowUVW(fragUv, flowVector, uvJump, _time, true, tiling);

        vec3 texColorA = texture(colorTexture, uvwA.xy).rrr * uvwA.z;
        vec3 texColorB = texture(colorTexture, uvwB.xy).rrr * uvwB.z;

        vec3 normalA = texture(normalMap, uvwA.xy).rgb * 2.0 - 1.0;
        vec3 normalB = texture(normalMap, uvwB.xy).rgb * 2.0 - 1.0;

        vec3 normal = normalize(normalA + normalB);

        outputColor = vec4(texColorA + texColorB, 1);
    }
    )";
    waterTileShader.name = "water tile Shader 2";
    waterTileShader.isLit = true;
    waterTileShader.isDepthPass = false;
    waterTileShader.SetId(1);
    waterTileShader.Compile(); 

    return waterTileShader;
}
}
}