#include "waterTileShader_1.hpp"

namespace KikooRenderer {

namespace CoreEngine {
Shader GetWaterTile_1Shader() {
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

    out vec4 clipSpace;
    out vec2 fragUv;
    out vec4 fragPos;

    const float tiling = 6.0;
    

    void main() {
        fragPos = modelMatrix * vec4(position.x, position.y, position.z, 1.0f);

        fragUv = uv * tiling;
        clipSpace =  modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
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

    uniform sampler2D reflectionTexture;
    uniform sampler2D refractionTexture;
    uniform sampler2D dudvmap;
    uniform sampler2D normalMap;
    uniform vec3 cameraPos;

    uniform LightSource lights[4];
    uniform int numLights; 

    in vec4 clipSpace;
    in vec2 fragUv;
    in vec4 fragPos;

    uniform float moveFactor;
    
    const float waveStrength = 0.02;
    const float reflectivity = 10;
    const float blueness = 0.3;

    void main()
    {
        vec2 distortedTexCoords = texture(dudvmap, vec2(fragUv.x + moveFactor, fragUv.y)).rg*0.1;
        distortedTexCoords = fragUv + vec2(distortedTexCoords.x, distortedTexCoords.y+moveFactor);
        vec2 totalDistortion = (texture(dudvmap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;        

        vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
        vec2 refractTexCoords = ndc.xy + totalDistortion;
        vec2 reflectTexCoords = vec2(ndc.x, -ndc.y) + totalDistortion;

        refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);
        reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
        reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

        vec4 reflectionColor = texture(reflectionTexture, reflectTexCoords);
        vec4 refractionColor = texture(refractionTexture, refractTexCoords);

        float refractiveFactor = dot(fragToCam, vec3(0, 1, 0));
        refractiveFactor = pow(refractiveFactor, reflectivity);

        vec4 normalColor = texture(normalMap, distortedTexCoords);
        vec3 normal = vec3(normalColor.r * 2.0 - 1.0, normalColor.b, normalColor.g * 2.0 - 1.0);
        normal = normalize(normal);

        vec3 fragToCam = normalize(cameraPos - fragPos.xyz);
        vec4 specularHighlights = vec4(0, 0, 0, 0);
        for(int i=0; i<numLights; i++) {
            vec3 lightDirection = normalize(lights[i].direction);
            if(lights[i].type == 1) { //Point light
                lightDirection = normalize(fragPos.xyz - lights[i].position);
            }
            if(lights[i].type == 2) { //Spot light
                lightDirection  = normalize(fragPos.xyz - lights[i].position);
            }            
            vec3 fragToLight = -lightDirection;
            
            vec3 reflectedLight = reflect(normalize(lightDirection), normal);
            float specular = max(dot(reflectedLight, fragToCam), 0.0);
            specular = pow(specular, 20);
            specularHighlights += lights[i].color * specular;
        }

        outputColor = mix(reflectionColor, refractionColor, refractiveFactor);
        outputColor = mix(outputColor, vec4(0.0, 0.3, 0.5, 1.0), blueness) + vec4(specularHighlights.rgb, 0);
    }
    )";
    waterTileShader.name = "water tile Shader 1";
    waterTileShader.isLit = true;
    waterTileShader.isDepthPass = false;
    waterTileShader.SetId(1);
    waterTileShader.Compile();
    waterTileShader.shouldRecompile = false;

    return waterTileShader;
}
}
}