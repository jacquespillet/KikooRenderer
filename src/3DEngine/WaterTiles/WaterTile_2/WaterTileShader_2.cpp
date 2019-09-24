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

    out vec2 fragUv;
    out vec4 fragPos;

    void main() {
        fragPos = modelMatrix * vec4(position.x, position.y, position.z, 1.0f);

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
    uniform vec3 cameraPos;
    
    in vec2 fragUv;
    in vec4 fragPos;

    const vec2 uvJump = vec2(0.0, 0.0);
    const float tiling = 3;
    const float speed = 0.5;
    const float strength = 0.1;
    const float flowOffset = 0;
    const float constantHeightScale = 0.25;
    const float modulatedHeightScale = 0.75;

    vec3 UnpackDerivativeHeight (vec4 textureData) {
        vec3 dh = textureData.agb;
        dh.xy = dh.xy * 2 - 1;
        return dh;
    }    
    
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
        outputColor = vec4(0, 0, 0, 1);

        vec3 flowVector = texture(flowMap, fragUv).rgb;
        flowVector.xy = flowVector.xy * 2 - 1;
        flowVector *= strength;        
        
        float noise = texture(flowMap, fragUv).a;
        float _time = time * speed + noise;    

        vec3 uvwA = flowUVW(fragUv, flowVector.xy, uvJump, _time, false, tiling);
        vec3 uvwB = flowUVW(fragUv, flowVector.xy, uvJump, _time, true, tiling);

        vec3 texColorA = texture(colorTexture, uvwA.xy).rrr * uvwA.z;
        vec3 texColorB = texture(colorTexture, uvwB.xy).rrr * uvwB.z;

        float finalHeightScale = flowVector.z * modulatedHeightScale + constantHeightScale;

        vec3 dhA = UnpackDerivativeHeight(texture(normalMap, uvwA.xy)) * (finalHeightScale * uvwA.z);
        vec3 dhB = UnpackDerivativeHeight(texture(normalMap, uvwB.xy)) * (finalHeightScale * uvwB.z);
        vec3 normal =  normalize(vec3(-(dhA.xy + dhB.xy), 1).xzy);


        vec4 specularHighlights = vec4(0, 0, 0, 0);
        vec4 diffuse = vec4(0, 0, 0, 0);
        vec3 fragToCam = normalize(cameraPos - fragPos.xyz);
        for(int i=0; i<numLights; i++) {
            vec3 lightDirection = normalize(lights[i].direction);
            if(lights[i].type == 1) { //Point light
                lightDirection = normalize(fragPos.xyz - lights[i].position);
            }
            if(lights[i].type == 2) { //Spot light
                lightDirection  = normalize(fragPos.xyz - lights[i].position);
            }            
            vec3 fragToLight = -lightDirection;
            diffuse += 0.1 * vec4(0.305, 0.513, 0.658, 0) * max(dot(normal.xyz, fragToLight), 0);

            vec3 halfwayVec = normalize(fragToLight + fragToCam);
            specularHighlights +=  vec4(1, 1, 1, 0) * pow(max(dot(normal.xyz, halfwayVec),0), 128);
        }        
   
        vec3 albedo = vec3(0.305, 0.513, 0.658);
        outputColor = vec4(albedo.xyz, 1) * vec4(texColorA + texColorB, 1) + diffuse + specularHighlights;
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