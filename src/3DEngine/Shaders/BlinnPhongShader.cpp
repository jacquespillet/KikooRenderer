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

    QCheckBox* ReflectSkyboxCheckbox = new QCheckBox("Reflect/Refract Skybox");
    shaderParamsLayout->addWidget(ReflectSkyboxCheckbox);
    connect(ReflectSkyboxCheckbox, &QCheckBox::stateChanged, this, [this](int state) {
        reflectSkybox = (state > 0);
        scene->triggerRefresh = true;
    });

    return shaderParamsLayout;
}

void BlinnPhongParams::SetUniforms() {
    GETGL

    /*
    * When textures changed in the form, load them for the incoming frame
    */
    if (shouldLoadNormal ) {
        normalMap = KikooRenderer::CoreEngine::Texture(normalMapStr, GL_TEXTURE2);
        shouldLoadNormal = false;
    }

    if (shouldLoadSpecular ) {
        specularMap = KikooRenderer::CoreEngine::Texture(specularMapStr, GL_TEXTURE1);
        shouldLoadSpecular = false;
    }
    
    /*
    Set variables for normal mapping : 
        * Texture variable
        * bool hasNormalTex
        * Influence of normals of the map on the actual normal
    */
    if(normalMap.loaded) {
        normalMap.Use();
        int texLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "normalTexture"); 
        ogl->glUniform1i(texLocation, 2); 
        
        int hasNormalTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasNormalTex"); 
        ogl->glUniform1i(hasNormalTexLocation, 1);

        int normalMapInfluenceLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "normalMapInfluence");
        ogl->glUniform1f(normalMapInfluenceLocation, normalMapInfluence);
    }  else {
        int hasNormalTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasNormalTex"); 
        ogl->glUniform1i(hasNormalTexLocation, 0);
    } 

    /*
    Set variables for Specular mapping : 
        * Texture variable
        * bool hasSpecularTex
    */
    if(specularMap.loaded) {
        specularMap.Use();
        int texLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "specularTexture"); 
        ogl->glUniform1i(texLocation, 1); 

        int hasSpecularTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasSpecularTex"); 
        ogl->glUniform1i(hasSpecularTexLocation, 1);
    } else {
        int hasSpecularTexLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "hasSpecularTex"); 
        ogl->glUniform1i(hasSpecularTexLocation, 0);
    }

    // std::cout << "here"<<std::endl;
    int reflectSkyboxInt = reflectSkybox ? 1 : 0;
    int reflectSkyboxLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "reflectSkybox"); 
    ogl->glUniform1i(reflectSkyboxLocation, reflectSkyboxInt);
    if (reflectSkybox) {
    	MaterialComponent* skyboxMaterial = (MaterialComponent*) scene->skyboxCube->GetComponent("Material");
        skyboxMaterial->cubemap.Use();
    	ogl->glActiveTexture(GL_TEXTURE3);
    	int cubemapLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "cubemapTexture");
    	ogl->glUniform1i(cubemapLocation, 3);
    }

    /*
    * Set all factors for computing blinn phong shading
    */
    

    int specularColorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "specularColor"); 
    ogl->glUniform4fv(specularColorLocation, 1, glm::value_ptr(specularColor));

    int ambientFactorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "ambientFactor");
    ogl->glUniform1f(ambientFactorLocation, ambientFactor);

    int diffuseFactorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "diffuseFactor");
    ogl->glUniform1f(diffuseFactorLocation, diffuseFactor);

    int specularFactorLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "specularFactor");
    ogl->glUniform1f(specularFactorLocation, specularFactor);

    int smoothnessLocation = ogl->glGetUniformLocation(this->shader->programShaderObject, "smoothness");
    ogl->glUniform1f(smoothnessLocation, smoothness);
}

std::string DirectionalShadowCalculation = R"(
    float DirectionalShadowCalculation(vec4 fragPosLightSpace, int inx)
    {
        vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
        projCoords = projCoords * 0.5 + 0.5;
        float currentDepth = projCoords.z;

        float bias = max(0.05 * (dot(fragNormal, normalize(lights[inx].direction))), 0.005);  
        
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

)";

std::string PointShadowCalculation = R"(
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

)";

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

    uniform int flipNormals;

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
        fragNormal = (flipNormals > 0) ? -mat3(transpose(inverse(modelMatrix))) * normal  : mat3(transpose(inverse(modelMatrix))) * normal;
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
    )" 
    +  
    DirectionalShadowCalculation
    + 
    PointShadowCalculation
    +
    R"(
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
                attenuation    = 1 / (lights[i].attenuation.x + lights[i].attenuation.y * lightDist + lights[i].attenuation.z * (lightDist * lightDist));
                lightDirection = normalize(fragPos.xyz - lights[i].position);
            }
            if(lights[i].type == 2) { //Spot light
                lightDirection  = normalize(fragPos.xyz - lights[i].position);
                lightDist  = distance(fragPos.xyz, lights[i].position);
                float numerator = pow(max(dot(-normalize(lights[i].direction), -lightDirection), 0), 64);
                attenuation     = numerator / (lights[i].attenuation.x + lights[i].attenuation.y * lightDist + lights[i].attenuation.z * (lightDist * lightDist));
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
            if(lights[i].type == 0) {
                vec4 fragPosLightSpace = lights[i].lightSpaceMatrix * vec4(fragPos, 1.0);
                shadow = DirectionalShadowCalculation(fragPosLightSpace, i);
            } else if(lights[i].type == 1) {
                shadow = PointShadowCalculation(fragPos, i);
            }
            finalColor.rgb += (1.0 - shadow) *  attenuation * (diffuse + specular).rgb;            
        }
        outputColor = finalColor;
        outputColor.a = finalAlbedo.a;
    }
    )";

    return blinnPhongShader;
}
}
}
