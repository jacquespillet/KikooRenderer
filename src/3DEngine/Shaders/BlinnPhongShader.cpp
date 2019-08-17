#include "BlinnPhongShader.hpp"


namespace KikooRenderer
{
namespace CoreEngine
{

Shader GetBlinnPhongShader() {
    Shader blinnPhongShader;

    blinnPhongShader.vertSrc= R"(
    #version 440

    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal;
    layout(location = 2) in vec2 uv;
    layout(location = 3) in vec4 color;
    layout(location = 4) in vec4 tangent;

    uniform mat4 modelViewProjectionMatrix;
    uniform mat4 modelMatrix;
    uniform vec3 cameraPos;

    out vec3 fragPos;
    out vec3 fragNormal;
    out vec2 fragUv;
    out vec4 fragTangent;
    out vec3 toCamTangentSpace; 
    out vec3 toLightTangentSpace; 

    void main()
    {
        vec4 finalPosition = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
        gl_Position = vec4(finalPosition.x, finalPosition.y, finalPosition.z, finalPosition.w);
        fragPos = (modelMatrix * vec4(position.x, position.y, position.z, 1.0f)).xyz;
        fragNormal = normal;
        fragUv = uv;
        fragTangent = tangent;
    }
    )";

    blinnPhongShader.fragSrc = R"(
    #version 440

    struct LightSource
    {
            int type;
            vec3 position;
            vec3 attenuation;
            vec3 direction;
            vec4 color;
    };

    layout(location = 0) out vec4 outputColor; 
    uniform LightSource lights[4];
    uniform vec4 albedo; 
    uniform int numLights; 
    uniform vec3 cameraPos;

    uniform sampler2D albedoTexture;
    uniform sampler2D specularTexture;
    uniform sampler2D normalTexture;
    uniform int hasAlbedoTex;
    uniform int hasSpecularTex;
    uniform int hasNormalTex;

    uniform float ambientFactor;
    uniform float diffuseFactor;
    uniform float specularFactor;
    uniform float smoothness;

    in vec3 fragPos;
    in vec3 fragNormal;
    in vec2 fragUv;
    in vec4 fragTangent;
    in vec3 toCamTangentSpace; 

    void main()
    {
        vec3 fragToCam = cameraPos - fragPos;

        vec4 finalAlbedo   = (hasAlbedoTex==1) ? albedo * texture(albedoTexture, fragUv) : albedo;
        vec3 finalNormal   = (hasNormalTex==1) ? texture(normalTexture, fragUv).xyz : fragNormal;
        vec4 specularColor = vec4(1, 1, 1, 1);
        
        float finalSpecularFactor = (hasSpecularTex==1) ? texture(specularTexture, fragUv).x : specularFactor;
        
        vec4 finalColor = vec4(0, 0, 0, 1);
        finalColor.rgb += ambientFactor * finalAlbedo.rgb;

        vec3 fragBitangent = normalize(cross(fragNormal, fragTangent.xyz) * fragTangent.w); 

        for(int i=0; i<numLights; i++) {
            float attenuation = 1;
            vec3 lightDirection = normalize(lights[i].direction);
            
            if(lights[i].type == 1) { //Point light
                float distance = distance(fragPos.xyz, lights[i].position);
                attenuation    = 1 / (lights[i].attenuation.x + lights[i].attenuation.y * distance + lights[i].attenuation.z * (distance * distance));
                lightDirection = normalize(fragPos.xyz - lights[i].position);
            }
            if(lights[i].type == 2) { //Spot light
                lightDirection  = normalize(fragPos.xyz - lights[i].position);
                float distance  = distance(fragPos.xyz, lights[i].position);
                float numerator = pow(max(dot(-normalize(lights[i].direction), -lightDirection), 0), 64);
                attenuation     = numerator / (lights[i].attenuation.x + lights[i].attenuation.y * distance + lights[i].attenuation.z * (distance * distance));
            }
            
            vec3 toLight = -lightDirection;
            //Redefinition of vectors in tangent space
            if(hasNormalTex == 1) {
                toLight     = normalize(vec3(dot(fragTangent.xyz, toLight),dot(fragBitangent, toLight), dot(fragNormal, toLight)));
                fragToCam   = normalize(vec3(dot(fragTangent.xyz, fragToCam), dot(fragBitangent, fragToCam), dot(fragNormal, fragToCam)));
            }


            vec4 diffuse = diffuseFactor * finalAlbedo * lights[i].color * max(dot(normalize(finalNormal.xyz), toLight), 0);

            vec3 halfwayVec = normalize(toLight + fragToCam);

            // Specular factor * specular color * lightColor * specularity of fragment
            vec4 specular = finalSpecularFactor * specularColor * lights[i].color * pow(max(dot(normalize(finalNormal.xyz), halfwayVec),0), smoothness);

            finalColor.rgb +=  attenuation * (diffuse + specular).rgb;  
        }
        outputColor = finalColor;
        outputColor.a = finalAlbedo.a;
    }
    )";

    return blinnPhongShader;
}
}
}
