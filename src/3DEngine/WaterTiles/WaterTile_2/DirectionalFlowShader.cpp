#include "waterTileShader_2.hpp"
#include "3DEngine/StandardShaders.hpp"

namespace KikooRenderer {

namespace CoreEngine {
Shader GetDirectionalFlowShader() {
    Shader directionalFlowShader;

    directionalFlowShader.vertSrc= R"(
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

    directionalFlowShader.fragSrc = R"(
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

    uniform float tiling;
    uniform float speed;
    uniform float strength;
    uniform float constantHeightScale;
    uniform float modulatedHeightScale;
    uniform float gridResolution;
    uniform float modulatedTiling;
    uniform int dualGrid;

    vec3 UnpackDerivativeHeight (vec4 textureData) {
        vec3 dh = textureData.agb;
        dh.xy = dh.xy * 2 - 1;
        return dh;
    } 
    
    vec2 flowUVW(vec2 uv, vec3 flowVector, float _tiling, float time, out mat2 rotation) {
        vec2 dir = normalize(flowVector.xy);
    	rotation = mat2(dir.y, dir.x, -dir.x, dir.y);    
        uv = mat2(dir.y, dir.x, -dir.x, dir.y) *  uv;        

        uv.y -= time * flowVector.z;
        return uv * _tiling;
    }

    vec3 FlowCell (vec2 uv, vec2 offset, float time, bool gridB) {
        vec2 shift = 1 - offset;
        shift *= 0.5;

        offset *= 0.5;
        if (gridB) {
            offset += 0.25;
            shift -= 0.25;
        }

        mat2 derivRotation;
        vec2 uvTiled = (floor(uv * gridResolution + offset) + shift) / gridResolution;        

        vec3 flow = texture(flowMap,uvTiled).rgb;
        flow.xy = flow.xy * 2 - 1;
        flow.z *= strength;
        
        float _tiling = flow.z * modulatedTiling + tiling;

        vec2 uvFlow = flowUVW(
            uv + offset, flow, _tiling, time, derivRotation
        );        

        vec3 dh = UnpackDerivativeHeight(texture(normalMap, uvFlow));
        dh.xy = derivRotation * dh.xy;
        dh *= flow.z * modulatedHeightScale + constantHeightScale;        
        return dh;
    }

    vec3 FlowGrid (vec2 uv, float _time, bool gridB) {
        vec3 dhA = FlowCell(uv, vec2(0, 0), _time, gridB);
        vec3 dhB = FlowCell(uv, vec2(1, 0), _time, gridB);
        vec3 dhC = FlowCell(uv, vec2(0, 1), _time, gridB);
        vec3 dhD = FlowCell(uv, vec2(1, 1), _time, gridB);

        vec2 t = uv * gridResolution;
        if (gridB) {
            t += 0.25;
        }
        t = abs(2 * fract(t) - 1);
        

        float wA = (1 - t.x) * (1 - t.y);
        float wB = t.x * (1 - t.y);
        float wC = (1 - t.x) * t.y;
        float wD = t.x * t.y;

        return dhA * wA + dhB * wB + dhC * wC + dhD * wD;        
    }
 
    void main(){
        outputColor = vec4(0, 0, 0, 1);
        vec3 normal = vec3(0, 1, 0);
        vec3 albedo = vec3(0.305, 0.513, 0.658);

        float _time = time * speed;
        vec2 uv = fragUv;

        vec3 dh = FlowGrid(uv, _time, false);
        if(dualGrid > 0) {
            dh = 0.5 * (dh + FlowGrid(uv, _time, true));
        }


        albedo = dh.z * dh.z * albedo;
        normal =  normalize(vec3(-dh.xy, 1).xzy);


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
   
        outputColor = vec4(albedo.xyz, 1) + diffuse + specularHighlights;
    }
    )"; 
    directionalFlowShader.name = "water tile Shader 2";
    directionalFlowShader.isLit = true;
    directionalFlowShader.isDepthPass = false;
    directionalFlowShader.SetId(SHADER_IDS::OTHER);
    directionalFlowShader.Compile(); 
    directionalFlowShader.shouldRecompile = false;
    
    return directionalFlowShader;
}
}
}