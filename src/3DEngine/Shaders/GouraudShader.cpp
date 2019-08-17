#include "GouraudShader.hpp"

namespace KikooRenderer
{
namespace CoreEngine
{

void GetGouraudShader(Shader* gouraudShader) {
    gouraudShader->vertSrc= R"(
        #version 440

        struct LightSource
        {
                int type;
                vec3 position;
                vec3 attenuation;
                vec3 direction;
                vec4 color;
        };

        layout(location = 0) in vec3 position;
        layout(location = 1) in vec3 normal;
        layout(location = 2) in vec2 uv;
        layout(location = 3) in vec4 color;

        //transforms
        uniform mat4 modelViewProjectionMatrix;
        uniform mat4 modelMatrix;

        uniform vec4 albedo; 
        uniform vec3 cameraPos;

        uniform LightSource lights[4];
        uniform int numLights;

        uniform sampler2D albedoTexture;
        uniform sampler2D specularTexture;
        uniform sampler2D normalTexture;

        uniform int hasNormalTex;
        uniform int hasAlbedoTex;
        uniform int hasSpecularTex;

        uniform float ambientFactor;
        uniform float diffuseFactor;
        uniform float specularFactor;
        uniform float smoothness;

        out vec4 fragColor;
        void main()
        {
            vec4 finalPosition = modelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0f);
            vec4 worldPosition = modelMatrix * vec4(position.x, position.y, position.z, 1.0f);
            
            vec3 fragToCam = cameraPos - worldPosition.xyz;

            vec4 mainAlbedo = albedo;
            vec3 mainNormal = normal;

            vec4 finalColor = vec4(0, 0, 0, 1);
            finalColor.rgb += ambientFactor * mainAlbedo.rgb;

            for(int i=0; i<numLights; i++) {
                float attenuation = 1;
                vec3 lightDirection = normalize(lights[i].direction);
                
                vec3 fragToLight = -lightDirection;

                vec4 diffuse = diffuseFactor * mainAlbedo * lights[i].color * max(dot(mainNormal, fragToLight ), 0);

                vec3 halfwayVec = normalize(fragToLight  + fragToCam);
                vec4 specular = specularFactor * mainAlbedo * lights[i].color * pow(max(dot(normal, halfwayVec),0), smoothness);

                finalColor.rgb +=  attenuation * (diffuse + specular ).rgb;
            }

            fragColor = finalColor;
            gl_Position = vec4(finalPosition.x, finalPosition.y, finalPosition.z, finalPosition.w);
        }
        )";

    gouraudShader->fragSrc = R"(
        #version 440
        layout(location = 0) out vec4 outputColor; 

        in vec4 fragColor;
        
        void main()
        {
            outputColor = fragColor;
            // outputColor = vec4(1, 0, 0, 1);
        }
        )";
}

}
}