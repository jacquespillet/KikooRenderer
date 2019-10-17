#version 440
vec2 poissonDisk[64] = vec2[]( 
    vec2(-0.613392, 0.617481),
    vec2(0.170019, -0.040254),
    vec2(-0.299417, 0.791925),
    vec2(0.645680, 0.493210),
    vec2(-0.651784, 0.717887),
    vec2(0.421003, 0.027070),
    vec2(-0.817194, -0.271096),
    vec2(-0.705374, -0.668203),
    vec2(0.977050, -0.108615),
    vec2(0.063326, 0.142369),
    vec2(0.203528, 0.214331),
    vec2(-0.667531, 0.326090),
    vec2(-0.098422, -0.295755),
    vec2(-0.885922, 0.215369),
    vec2(0.566637, 0.605213),
    vec2(0.039766, -0.396100),
    vec2(0.751946, 0.453352),
    vec2(0.078707, -0.715323),
    vec2(-0.075838, -0.529344),
    vec2(0.724479, -0.580798),
    vec2(0.222999, -0.215125),
    vec2(-0.467574, -0.405438),
    vec2(-0.248268, -0.814753),
    vec2(0.354411, -0.887570),
    vec2(0.175817, 0.382366),
    vec2(0.487472, -0.063082),
    vec2(-0.084078, 0.898312),
    vec2(0.488876, -0.783441),
    vec2(0.470016, 0.217933),
    vec2(-0.696890, -0.549791),
    vec2(-0.149693, 0.605762),
    vec2(0.034211, 0.979980),
    vec2(0.503098, -0.308878),
    vec2(-0.016205, -0.872921),
    vec2(0.385784, -0.393902),
    vec2(-0.146886, -0.859249),
    vec2(0.643361, 0.164098),
    vec2(0.634388, -0.049471),
    vec2(-0.688894, 0.007843),
    vec2(0.464034, -0.188818),
    vec2(-0.440840, 0.137486),
    vec2(0.364483, 0.511704),
    vec2(0.034028, 0.325968),
    vec2(0.099094, -0.308023),
    vec2(0.693960, -0.366253),
    vec2(0.678884, -0.204688),
    vec2(0.001801, 0.780328),
    vec2(0.145177, -0.898984),
    vec2(0.062655, -0.611866),
    vec2(0.315226, -0.604297),
    vec2(-0.780145, 0.486251),
    vec2(-0.371868, 0.882138),
    vec2(0.200476, 0.494430),
    vec2(-0.494552, -0.711051),
    vec2(0.612476, 0.705252),
    vec2(-0.578845, -0.768792),
    vec2(-0.772454, -0.090976),
    vec2(0.504440, 0.372295),
    vec2(0.155736, 0.065157),
    vec2(0.391522, 0.849605),
    vec2(-0.620106, -0.328104),
    vec2(0.789239, -0.419965),
    vec2(-0.545396, 0.538133),
    vec2(-0.178564, -0.596057)
);

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
    int shadowType;
    int numSamples;
    int poissonDiskFactor;
    float bias;
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
    float samples = 4;
    float offset  = 1 / float(lights[inx].numSamples);
    for(float x = -offset; x < offset; x += offset / (samples * 0.5))
    {
        for(float y = -offset; y < offset; y += offset / (samples * 0.5))
        {
            for(float z = -offset; z < offset; z += offset / (samples * 0.5))
            {
                float closestDepth = texture(lights[inx].depthCubeMap, fragToLight + vec3(x, y, z)).r; 
                closestDepth *= lights[inx].farPlane;   // Undo mapping [0;1]
                if(currentDepth - lights[inx].bias > closestDepth)
                    shadow += 1.0;
            }
        }
    }
    shadow /= (samples * samples * samples);    
    return shadow;
}

float DirectionalShadowCalculation(vec4 fragPosLightSpace, int inx)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (dot(fragNormal, normalize(lights[inx].direction))), lights[inx].bias);  
    // bias = 0.00005;
    float shadow = 0;

    
    if(lights[inx].shadowType == 0) {// PCF
        vec2 texelSize = 1.0 / textureSize(lights[inx].depthMap, 0);
        int numSamples = lights[inx].numSamples;
        int counter = 0;

        for(int x = -numSamples; x <= numSamples; ++x)
        {
            for(int y = -numSamples; y <= numSamples; ++y)
            {
                float closestDepth = texture(lights[inx].depthMap, projCoords.xy + vec2(x, y) * texelSize).r; 
                if(currentDepth - lights[inx].bias > closestDepth) {
                    shadow += 1.0;                
                }
                counter++;
            }    
        }
        shadow /= counter;
    } else if(lights[inx].shadowType == 1) { //POISSON DISK SAMPLING

        int numSamples = min(64, lights[inx].numSamples);
        float numSamplesInv = 1.0 / float(numSamples);
        for (int i=0;i<numSamples;i++){
            int index = int(float(numSamples) * randomNumber(vec4(gl_FragCoord.xyy, i)));
            float depth = texture(lights[inx].depthMap, projCoords.xy + poissonDisk[index]/lights[inx].poissonDiskFactor).r; 
            if ( depth  <  currentDepth - lights[inx].bias ){
                shadow+=numSamplesInv;
            }
        }

        if(projCoords.z > 1.0)
            shadow = 0.0;
    }


    return max(0, shadow);
} 

void main()
{
    vec3 fragToCam = cameraPos - fragPos;

    vec4 finalAlbedo   = (hasAlbedoTex==1) ? albedo * texture(albedoTexture, fragUv) : albedo;
    
    vec3 finalNormal   = vec3(0);
    if(hasNormalTex==1) {
        finalNormal = (texture(normalTexture, fragUv).xyz - 0.5) * 2.0;
        vec3 fragBitangent = normalize(cross(fragNormal, fragTangent.xyz) * fragTangent.w); 
        mat4 theMatrix;
        theMatrix[0] = vec4(normalize(fragTangent.xyz), 1);
        theMatrix[1] = -vec4(normalize(fragBitangent), 1);
        theMatrix[2] = vec4(normalize(fragNormal), 1);
        theMatrix[3] = vec4(0, 0, 0, 1);
        theMatrix = inverse(theMatrix);
        finalNormal = (inverse(transpose(modelMatrix)) * theMatrix * vec4(finalNormal.xyz, 0)).xyz;
    } else {
        finalNormal = (inverse(transpose(modelMatrix)) * normalize(vec4(fragNormal.xyz, 0))).xyz;
    }
    

    float finalSpecularFactor = (hasSpecularTex==1) ? texture(specularTexture, fragUv).x : specularFactor;
    
    vec4 finalSpecularColor = specularColor;
    if(reflectSkybox == 1) {
        vec3 I = -normalize(fragToCam);
        vec3 R = vec3(0, 0, 0);
        if(finalAlbedo.a < 1) {
            float ratio = 1.00 / 1.52;
            R = refract(I, normalize(finalNormal), ratio);
        } else {
            R = reflect(I, normalize(finalNormal));
        }
        finalSpecularColor = vec4(texture(cubemapTexture, R).rgb, 1.0);
    }

    vec4 finalColor = vec4(0, 0, 0, 1);
    finalColor.rgb += ambientFactor * finalAlbedo.rgb;

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

        vec4 diffuse = diffuseFactor * finalAlbedo * lights[i].color * max(dot(finalNormal.xyz, toLight), 0);

        vec3 halfwayVec = normalize(toLight + fragToCam);
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