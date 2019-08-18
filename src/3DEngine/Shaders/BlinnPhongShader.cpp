#include "BlinnPhongShader.hpp"


namespace KikooRenderer
{
namespace CoreEngine
{

/*
* Returns a layout containing all the inputs for blinn phong shading
*/
 QLayout* BlinnPhongParams::GetLayout() {
    QVBoxLayout* shaderParamsLayout = new QVBoxLayout();

    
    ColorPicker* specularColorPicker = new ColorPicker("specular Color", 210, 15, 60, 255);
    shaderParamsLayout->addWidget(specularColorPicker);
    connect(specularColorPicker, &ColorPicker::ColorPicked, this, [this](QColor color) {
        glm::vec4 specularColorVec = glm::dvec4(color.red(), color.green(), color.blue(), color.alpha()) * 0.00392156;
        specularColor = specularColorVec;
        scene->triggerRefresh = true;
    });

    // Normal Map
    FilePicker* normalMapPicker = new FilePicker("Normal Map", normalMapStr);
    shaderParamsLayout->addWidget(normalMapPicker);
    connect(normalMapPicker, &FilePicker::FileModified, this, [this](QString string) {
        normalMapStr = string.toStdString();
        shouldLoadNormal = true;
        scene->triggerRefresh = true;
    });

    CustomSlider* normalMapInfluenceSlider = new CustomSlider(0.0f, 3.0f, 0.01, "Normal Map Influence", normalMapInfluence);
    shaderParamsLayout->addLayout(normalMapInfluenceSlider);
    QObject::connect(normalMapInfluenceSlider, &CustomSlider::Modified, [this](double val) {
        normalMapInfluence = val;
        scene->triggerRefresh = true;
    });

    //Specular Map
    FilePicker* specularPicker = new FilePicker("Specular Map", specularMapStr);
    shaderParamsLayout->addWidget(specularPicker);
    connect(specularPicker, &FilePicker::FileModified, this, [this](QString string) {
        specularMapStr = string.toStdString();
        shouldLoadSpecular = true;
        scene->triggerRefresh = true;
    });

    CustomSlider* ambientSlider = new CustomSlider(0, 1, 0.01, "Ambient Factor", ambientFactor);
    shaderParamsLayout->addLayout(ambientSlider);
    QObject::connect( ambientSlider, &CustomSlider::Modified, [this](double val) {
        ambientFactor = val;
        scene->triggerRefresh = true;
    });

    CustomSlider* diffuseSlider = new CustomSlider(0, 1, 0.01, "Diffuse Factor", diffuseFactor);
    shaderParamsLayout->addLayout(diffuseSlider);
    QObject::connect(diffuseSlider, &CustomSlider::Modified, [this](double val) {
        diffuseFactor = val;
        scene->triggerRefresh = true;
    });

    CustomSlider* specularSlider = new CustomSlider(0, 1, 0.01, "Specular Factor", specularFactor);
    shaderParamsLayout->addLayout(specularSlider);
    QObject::connect(specularSlider, &CustomSlider::Modified, [this](double val) {
        specularFactor = val;
        scene->triggerRefresh = true;
    });

    CustomSlider* smoothnessSlider = new CustomSlider(0, 128, 1, "Smoothness", smoothness);
    shaderParamsLayout->addLayout(smoothnessSlider);
    QObject::connect(smoothnessSlider, &CustomSlider::Modified, [this](double val) {
        smoothness = val;
        scene->triggerRefresh = true;
    });

    return shaderParamsLayout;
}

/*
* Returns the actual blinn phong shader
*/
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
    uniform float normalMapInfluence;


    uniform float ambientFactor;
    uniform float diffuseFactor;
    uniform float specularFactor;
    uniform float smoothness;
    uniform vec4 specularColor;

    in vec3 fragPos;
    in vec3 fragNormal;
    in vec2 fragUv;
    in vec4 fragTangent;
    in vec3 toCamTangentSpace; 

    void main()
    {
        vec3 fragToCam = cameraPos - fragPos;

        vec4 finalAlbedo   = (hasAlbedoTex==1) ? albedo * texture(albedoTexture, fragUv) : albedo;
        vec3 finalNormal   = (hasNormalTex==1) ? normalMapInfluence * normalize(texture(normalTexture, fragUv)).xyz : fragNormal;
        // vec4 specularColor = vec4(1, 1, 1, 1);
        
        float finalSpecularFactor = (hasSpecularTex==1) ? texture(specularTexture, fragUv).x : specularFactor;
        
        vec4 finalColor = vec4(0, 0, 0, 1);
        finalColor.rgb += ambientFactor * finalAlbedo.rgb;

        //Multiply by handedness stored in w of tangent vector
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


            vec4 diffuse = diffuseFactor * finalAlbedo * lights[i].color * max(dot(finalNormal.xyz, toLight), 0);

            vec3 halfwayVec = normalize(toLight + fragToCam);

            // Specular factor * specular color * lightColor * specularity of fragment
            vec4 specular = finalSpecularFactor * specularColor * lights[i].color * pow(max(dot(finalNormal.xyz, halfwayVec),0), smoothness);

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
