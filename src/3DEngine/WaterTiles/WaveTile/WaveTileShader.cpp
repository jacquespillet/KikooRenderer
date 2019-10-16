#include "WaveTileShader.hpp"
#include "3DEngine/StandardShaders.hpp"

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
    uniform mat4 viewProjectionMatrix;
    uniform float time;

    out vec4 clipSpace;
    out vec2 fragUv;
    out vec3 fragPos;
    out vec3 fragNormal;

    const float PI = 3.14159265358979;
    
    //dir, steepness, wavelength
    uniform vec4 waves[20];
    uniform int numWaves;


    vec3 GerstnerWave(vec4 wave, vec3 p, inout vec3 tangent, inout vec3 binormal) {
        float k = 2 * PI / wave.w;
        float c = sqrt(9.8 / k);
        vec2 d = normalize(wave.xy);
        float f = k * (dot(d, p.xz)- c * time);
        float a = wave.z / k;
        
        tangent += vec3(
            1 - d.x * d.x * (wave.z * sin(f)),
            d.x * (wave.z * cos(f)),
            -d.x * d.y * (wave.z * sin(f))
        );
        binormal += vec3(
            -d.x * d.y * (wave.z * sin(f)),
            d.y * (wave.z * cos(f)),
            1 - d.y * d.y * (wave.z * sin(f))
        );

        p.x = d.x * (a * cos(f));
        p.y = a * sin(f);
        p.z = d.y * (a * cos(f));

        return p;
    }

    void main() {
        vec3 p = (modelMatrix * vec4(position.x, position.y, position.z, 1)).xyz;
        vec3 tangent = vec3(1, 0, 0);
        vec3 binormal = vec3(0, 0, 1);

        for(int i=0; i<numWaves; i++) {
            p+= GerstnerWave(waves[i], p, tangent,binormal);
        }
  
  
        fragNormal = normalize(cross(binormal, tangent));
        fragPos = p;

        clipSpace =  viewProjectionMatrix * vec4(p.x, p.y, p.z, 1.0f);
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
    in vec3 fragPos;
    in vec3 fragNormal;

    void main()
    {
        vec3 normal = normalize(fragNormal);
        vec4 specularHighlights = vec4(0, 0, 0, 0);
        vec4 diffuse = vec4(0, 0, 0, 0);
        vec3 fragToCam = normalize(cameraPos - fragPos);
        for(int i=0; i<numLights; i++) {
            vec3 lightDirection = normalize(lights[i].direction);
            if(lights[i].type == 1) { //Point light
                lightDirection = normalize(fragPos - lights[i].position);
            }
            if(lights[i].type == 2) { //Spot light
                lightDirection  = normalize(fragPos - lights[i].position);
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
    waveTileShader.SetId(SHADER_IDS::OTHER);
    waveTileShader.Compile();
    waveTileShader.shouldRecompile = false;

    return waveTileShader;
}
}
}