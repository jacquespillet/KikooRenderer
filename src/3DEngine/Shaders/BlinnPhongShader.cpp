#include "BlinnPhongShader.hpp"


namespace KikooRenderer
{
namespace CoreEngine
{

Shader GetBlinnPhongShader() {
    Shader blinnPhongShader;


    // BlinnPhongParams* params = new BlinnPhongParams();
    // params->shader = &blinnPhongShader;
    // blinnPhongShader.SetParams(params);

    blinnPhongShader.vertSrc= R"(
    #version 440

    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal;
    layout(location = 2) in vec2 uv;
    layout(location = 3) in vec4 color;
    layout(location = 4) in vec3 tangent;

    uniform mat4 modelViewProjectionMatrix;
    uniform mat4 modelMatrix;
    uniform vec3 cameraPos;

    out vec3 fragPos;
    out vec3 fragNormal;
    out vec2 fragUv;
    out vec3 fragTangent;
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

        vec3 fragToCam = cameraPos - fragPos;
        vec3 bitangent = cross(normal, tangent); 
        toCamTangentSpace = vec3(dot(tangent, fragToCam), dot(bitangent, fragToCam), dot(normal, fragToCam));
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

    in vec3 fragPos;
    in vec3 fragNormal;
    in vec2 fragUv;
    in vec3 fragTangent;
    in vec3 toCamTangentSpace; 

    void main()
    {
        vec3 fragToCam = cameraPos - fragPos;

        vec4 finalAlbedo = (hasAlbedoTex==1) ? albedo * texture(albedoTexture, fragUv) : albedo;
        vec3 finalNormal = (hasNormalTex==1) ? texture(normalTexture, fragUv).xyz : fragNormal;
        
        vec4 finalColor = vec4(0, 0, 0, 1);
        finalColor.rgb += 0.1 * finalAlbedo.rgb;

        for(int i=0; i<numLights; i++) {
            vec3 fragBitangent = normalize(cross(fragNormal, fragTangent)); 
            vec3 toLightTangentSpace = vec3(dot(fragTangent, lights[i].direction),    dot(fragBitangent, lights[i].direction), dot(fragNormal, lights[i].direction)); 

            float attenuation = 1;
            vec3 lightDirection = normalize(lights[i].direction);
            
            if(lights[i].type == 1) { //Point light
                float distance = distance(fragPos.xyz, lights[i].position);
                attenuation = 1 / (lights[i].attenuation.x + lights[i].attenuation.y * distance + lights[i].attenuation.z * (distance * distance));
                lightDirection = normalize(fragPos.xyz - lights[i].position);
            }
            if(lights[i].type == 2) { //Spot light
                lightDirection = normalize(fragPos.xyz - lights[i].position);
                float distance = distance(fragPos.xyz, lights[i].position);
                float numerator = pow(max(dot(-normalize(lights[i].direction), -lightDirection), 0), 64);
                attenuation = numerator / (lights[i].attenuation.x + lights[i].attenuation.y * distance + lights[i].attenuation.z * (distance * distance));
            }
            
            vec3 toLight = -lightDirection;

            //Redefinition of vectors in tangent space
            if(hasNormalTex == 1) {
                toLight = toLightTangentSpace;
                fragToCam = toCamTangentSpace;
            }


            vec4 diffuse = 0.5 * finalAlbedo * lights[i].color * max(dot(normalize(finalNormal.xyz), toLight), 0);

            // Specular
            vec3 halfwayVec = normalize(toLight + fragToCam);
            vec4 specular = finalAlbedo * lights[i].color * pow(max(dot(normalize(finalNormal.xyz), halfwayVec),0), 64);

            finalColor.rgb +=  attenuation * (diffuse + specular).rgb;
        }
        // finalColor = vec4(fragTangent.xyz, 1);
        outputColor = finalColor;
        outputColor.a = finalAlbedo.a;
        
    }
    )";

    return blinnPhongShader;
}
}
}