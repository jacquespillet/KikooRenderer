#version 440

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

layout(location = 0) out vec4 outputColor; 
uniform LightSource lights[4];
uniform vec4 albedo; 
uniform int numLights; 
uniform vec3 cameraPos;

uniform sampler2D albedoTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;
uniform samplerCube cubemapTexture;
uniform int hasAlbedoTex;
uniform int hasSpecularTex;
uniform int hasNormalTex;
uniform float normalMapInfluence;
uniform int reflectSkybox;
uniform int receiveShadow;


uniform float ambientFactor;
uniform float diffuseFactor;
uniform float specularFactor;
uniform float smoothness;
uniform vec4 specularColor;
uniform mat4 modelMatrix;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragUv;
in vec4 fragTangent;
in vec3 toCamTangentSpace; 

float randomNumber(vec4 seed) {
    float dot_product = dot(seed, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453);
}

float PointShadowCalculation(vec3 fragPos, int inx){
    vec3 fragToLight = fragPos - lights[inx].position; 
    float currentDepth = length(fragToLight);  

    float shadow  = 0.0;
    float bias    = 0.05; 
    float samples = 4.0;
    float offset  = 0.1;
    for(float x = -offset; x < offset; x += offset / (samples * 0.5))
    {
        for(float y = -offset; y < offset; y += offset / (samples * 0.5))
        {
            for(float z = -offset; z < offset; z += offset / (samples * 0.5))
            {
                float closestDepth = texture(lights[inx].depthCubeMap, fragToLight + vec3(x, y, z)).r; 
                closestDepth *= lights[inx].farPlane;   // Undo mapping [0;1]
                if(currentDepth - bias > closestDepth)
                    shadow += 1.0;
            }
        }
    }
    shadow /= (samples * samples * samples);    

    //NO PCF
    // float closestDepth = texture(lights[inx].depthCubeMap, fragToLight).r;
    // closestDepth *= lights[inx].farPlane;
    // shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0; 

    return shadow;
}

float DirectionalShadowCalculation(vec4 fragPosLightSpace, int inx)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (dot(fragNormal, normalize(lights[inx].direction))), 0.005);  
    bias = 0.00005;
    float shadow = 0;

    ///////MANUAL PCF
    // vec2 texelSize = 1.0 / textureSize(lights[inx].depthMap, 0);
    // for(int x = -1; x <= 1; ++x)
    // {
    //     for(int y = -1; y <= 1; ++y)
    //     {
    //         float pcfDepth = texture(lights[inx].depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
    //         shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
    //     }    
    // }
    // shadow /= 9.0;


    //POISSON DISK SAMPLING
    vec2 poissonDisk[16] = vec2[]( 
        vec2( -0.94201624, -0.39906216 ), 
        vec2( 0.94558609, -0.76890725 ), 
        vec2( -0.094184101, -0.92938870 ), 
        vec2( 0.34495938, 0.29387760 ), 
        vec2( -0.91588581, 0.45771432 ), 
        vec2( -0.81544232, -0.87912464 ), 
        vec2( -0.38277543, 0.27676845 ), 
        vec2( 0.97484398, 0.75648379 ), 
        vec2( 0.44323325, -0.97511554 ), 
        vec2( 0.53742981, -0.47373420 ), 
        vec2( -0.26496911, -0.41893023 ), 
        vec2( 0.79197514, 0.19090188 ), 
        vec2( -0.24188840, 0.99706507 ), 
        vec2( -0.81409955, 0.91437590 ), 
        vec2( 0.19984126, 0.78641367 ), 
        vec2( 0.14383161, -0.14100790 ) 
    );
    for (int i=0;i<16;i++){
        int index = int(16.0*randomNumber(vec4(gl_FragCoord.xyy, i)));
        float depth = texture(lights[inx].depthMap, projCoords.xy + poissonDisk[index]/700.0).r; 
        if ( depth  <  currentDepth - bias ){
            shadow+=0.0625;
        }
    }

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return max(0, shadow);
} 

void main()
{
    vec3 fragToCam = cameraPos - fragPos;

    vec4 finalAlbedo   = (hasAlbedoTex==1) ? albedo * texture(albedoTexture, fragUv) : albedo;
    vec3 finalNormal   = (hasNormalTex==1) ? normalMapInfluence * normalize(texture(normalTexture, fragUv)).xyz : normalize(fragNormal);
    
    float finalSpecularFactor = (hasSpecularTex==1) ? texture(specularTexture, fragUv).x : specularFactor;
    
    vec4 finalSpecularColor = specularColor;
    if(reflectSkybox == 1) {
        vec3 I = -normalize(fragToCam);
        vec3 R = vec3(0, 0, 0);
        if(finalAlbedo.a < 1) {
            float ratio = 1.00 / 1.52;
            R = refract(I, normalize(fragNormal), ratio);
        } else {
            R = reflect(I, normalize(fragNormal));
        }
        finalSpecularColor = vec4(texture(cubemapTexture, R).rgb, 1.0);
    }

    vec4 finalColor = vec4(0, 0, 0, 1);
    finalColor.rgb += ambientFactor * finalAlbedo.rgb;

    vec3 fragBitangent = normalize(cross(fragNormal, fragTangent.xyz) * fragTangent.w); 

    for(int i=0; i<numLights; i++) {
        float attenuation = 1;
        vec3 lightDirection = normalize(lights[i].direction);
        
        float lightDist = 0;
        if(lights[i].type == 1) { //Point light
            lightDist = distance(fragPos.xyz, lights[i].position);
            attenuation    = 1 / (lights[i].attenuation.x * (lightDist * lightDist));
            lightDirection = normalize(fragPos.xyz - lights[i].position);
        }
        if(lights[i].type == 2) { //Spot light
            lightDirection  = normalize(fragPos.xyz - lights[i].position);
            lightDist  = distance(fragPos.xyz, lights[i].position);
            float numerator = pow(max(dot(-normalize(lights[i].direction), -lightDirection), 0), 64);
            attenuation     = numerator / (lights[i].attenuation.x * (lightDist * lightDist));
        }
        
        vec3 toLight = -lightDirection;

        //Redefinition of vectors in tangent space
        if(hasNormalTex == 1) {
            toLight     = normalize(vec3(dot(fragTangent.xyz, toLight),dot(fragBitangent, toLight), dot(fragNormal, toLight)));
            fragToCam   = normalize(vec3(dot(fragTangent.xyz, fragToCam), dot(fragBitangent, fragToCam), dot(fragNormal, fragToCam)));
        }

        vec4 diffuse = diffuseFactor * finalAlbedo * lights[i].color * max(dot(finalNormal.xyz, toLight), 0);

        vec3 halfwayVec = normalize(toLight + fragToCam);
        // Specular factor * specular color * lightColor * specularity of fragment
        vec4 specular = finalSpecularFactor * finalSpecularColor * lights[i].color * pow(max(dot(finalNormal.xyz, halfwayVec),0), smoothness);

        float shadow = 0;
        if(receiveShadow > 0) {
            if(lights[i].type == 0) {
                vec4 fragPosLightSpace = lights[i].lightSpaceMatrix * vec4(fragPos, 1.0);
                shadow = DirectionalShadowCalculation(fragPosLightSpace, i);
            } else if(lights[i].type == 1) {
                shadow = PointShadowCalculation(fragPos, i);
            }else if(lights[i].type == 2) {
                vec4 fragPosLightSpace = lights[i].lightSpaceMatrix * vec4(fragPos, 1.0);
                shadow = DirectionalShadowCalculation(fragPosLightSpace, i);
            }
        }
        finalColor.rgb += (1.0 - shadow) *  attenuation * (diffuse + specular).rgb;            
    }
    outputColor = finalColor;
    outputColor.a = finalAlbedo.a;
}