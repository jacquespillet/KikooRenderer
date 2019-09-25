#include "WaveTileShader.hpp"

namespace KikooRenderer {

namespace CoreEngine {
Shader GetWaveTileShader() {
    Shader waveTileShader;

    waveTileShader.vertSrc= R"(
    //attribs
    #version 440
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal;
    layout(location = 2) in vec2 uv;
    layout(location = 3) in vec4 color;

    uniform mat4 modelViewProjectionMatrix;
    uniform mat4 modelMatrix;
    uniform float time;

    out vec4 clipSpace;
    out vec2 fragUv;
    out vec4 fragPos;
    out vec3 fragNormal;

    const float PI = 3.14159265358979;
    
    //dir, steepness, wavelength
    const vec4 waveA = vec4(1, 0, 0.5, 10); 

    vec3 GerstnerWave(vec4 wave, vec3 p, inout vec3 tangent, inout vec3 binormal) {
        float steepness = wave.z;
        float wavelength = wave.w;
        float k = 2 * PI / wavelength;
        float c = sqrt(9.8 / k);
        vec2 d = normalize(wave.xy);
        float f = k * (dot(d, p.xz) - c * time);
        float a = steepness / k;
        
        //p.x += d.x * (a * cos(f));
        //p.y = a * sin(f);
        //p.z += d.y * (a * cos(f));

        tangent += vec3(
            -d.x * d.x * (steepness * sin(f)),
            d.x * (steepness * cos(f)),
            -d.x * d.y * (steepness * sin(f))
        );
        binormal += vec3(
            -d.x * d.y * (steepness * sin(f)),
            d.y * (steepness * cos(f)),
            -d.y * d.y * (steepness * sin(f))
        );
        return vec3(
            d.x * (a * cos(f)),
            a * sin(f),
            d.y * (a * cos(f))
        );        

    }

    void main() {
        vec3 p = (modelMatrix * vec4(position.x, position.y, position.z, 1.0f)).xyz;

        float k = 2 * PI / waveA.w;
        float c = sqrt(9.8 / k);
        vec2 d = normalize(waveA.xy);
        float f = k * (dot(d, p.xz)- c * time);
        float a = waveA.z / k;

        p.x += d.x * (a * cos(f));
        p.y = a * sin(f);
        p.z += d.y * (a * cos(f));
        
        vec3 tangent = vec3(
            1 - d.x * d.x * (waveA.z * sin(f)),
            d.x * (waveA.z * cos(f)),
            -d.x * d.y * (waveA.z * sin(f))
        );
        vec3 binormal = vec3(
            -d.x * d.y * (waveA.z * sin(f)),
            d.y * (waveA.z * cos(f)),
            1 - d.y * d.y * (waveA.z * sin(f))
        );
        fragNormal = normalize(cross(binormal, tangent));
        

        clipSpace =  modelViewProjectionMatrix * vec4(p.x, p.y, p.z, 1.0f);
        gl_Position = clipSpace;
    }
    )";

    waveTileShader.fragSrc = R"(
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

    uniform vec3 cameraPos;

    uniform LightSource lights[4];
    uniform int numLights; 

    in vec4 clipSpace;
    in vec2 fragUv;
    in vec4 fragPos;
    in vec3 fragNormal;

    void main()
    {
        vec3 normal = normalize(fragNormal);
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
            specularHighlights +=  vec4(1, 1, 1, 0) * pow(max(dot(normal.xyz, halfwayVec),0), 256);
        }   

        vec3 albedo = vec3(0.305, 0.513, 0.658);
        outputColor = vec4(albedo.rgb, 1) + specularHighlights + diffuse;
    }
    )";
    waveTileShader.name = "water tile Shader 1";
    waveTileShader.isLit = true;
    waveTileShader.isDepthPass = false;
    waveTileShader.SetId(1);
    waveTileShader.Compile();

    return waveTileShader;
}
}
}