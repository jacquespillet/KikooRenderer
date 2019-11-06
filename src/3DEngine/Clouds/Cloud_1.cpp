#include "Cloud_1.hpp"

#include "../BaseObjects.hpp"
#include "../Components/MaterialComponent.hpp"
#include "../Components/MeshFilterComponent.hpp"

#include "../Texture.hpp"
#include "Util/RandomUtil.hpp"
#include "Util/NoiseUtil.hpp"
#include "Util/Image.hpp"

#include "Util/ThreadingUtil.hpp"

namespace KikooRenderer {

namespace CoreEngine {

Cloud_1::Cloud_1(std::string name, Scene* scene) : Object3D(name, scene) {

    cloudShader.vertSrc= R"(
    //attribs
    #version 440
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal;
    layout(location = 2) in vec2 uv;
    layout(location = 3) in vec4 color;

    //outputs
    out vec2 fragmentUv;
    //main
    void main()
    {
        fragmentUv = vec2(uv.x, uv.y);
        gl_Position = vec4(position.x, position.y, position.z, 1.0);
    }
    )";

    cloudShader.fragSrc = R"(
    //inputs
    #version 440
    in vec2 fragmentUv;

    //output
    layout(location = 0) out vec4 outputColor; 
    uniform sampler2D albedoTexture;

    uniform mat4 camModelMatrix;
    uniform float camFov;
    uniform float camAspectRatio;

    uniform sampler3D noiseTex;

    uniform mat4 boxTransform;
    uniform vec3 boxScale;

    uniform float densityThreshold;
    uniform float densityFactor;
    uniform float frequency;

    uniform float channel0Influence;
    uniform float channel1Influence;
    uniform float channel2Influence;
    uniform float channel3Influence;

    // const vec3 lightPos = vec3(10, 10, 10);
    uniform vec3 lightPos;

    bool RayBoxTest(vec3 rayOrig, vec3 rayDir, mat4 transform, vec3 minScale,vec3 maxScale, out float distance)
    {
        //transform ray
        mat4 invTr = inverse(transform); 
        rayOrig = vec3( invTr * vec4(rayOrig, 1) );
        rayDir = vec3( invTr * vec4(rayDir, 0) );
        
        //AABB Ray intersection 
        vec3 rayDirInv = vec3(1.0f / rayDir.x, 1.0f / rayDir.y, 1.0f / rayDir.z); 
        vec3 t1 = (minScale - rayOrig) * rayDirInv;
        vec3 t2 = (maxScale - rayOrig) * rayDirInv;
        
        vec3 intersectionPosition = rayOrig + t2.x * rayDir;
        float tmin = (max)( (max)( (min)(t1.x, t2.x), (min)(t1.y, t2.y)), (min)(t1.z, t2.z));
        float tmax = (min)( (min)( (max)(t1.x, t2.x), (max)(t1.y, t2.y)), (max)(t1.z, t2.z));
        bool raybox = tmax >= (max)(0.0f, tmin);

        if(tmin < 0) {
            distance = tmax;
        } else {
            distance = tmin;
        }         
       
        if(!raybox)
            distance = -1; 
        
        return raybox;
    }

    float sampleDensity(vec3 position, out float lightDensity) {
        vec4 noiseSample = texture(noiseTex, position * frequency).rgba;
        
        float value = (noiseSample.r * channel0Influence)
                    * ( noiseSample.g * channel1Influence * 0.6
                    + noiseSample.b * channel2Influence * 0.3
                    + noiseSample.a * channel3Influence * 0.1);

        float heightFactor = 1 - (position.y / boxScale.y);
        value *= heightFactor;


        value = min(1, max(0, value - densityThreshold)) * densityFactor;
        


        //Light Computation
        //_________________________________
        // vec3 pointToLight = normalize(lightPos - position);
        // float toLightDistance=0;
        // RayBoxTest(position , pointToLight, boxTransform, vec3(-0.5),vec3(0.5), toLightDistance);

        // float sampleSize = toLightDistance;
        // float numSteps = 10.0f;
        // float stepSize = sampleSize / numSteps;

        // lightDensity = 0;
        // for(float i=0; i<numSteps; i++) {
        //     vec3 newPos = position + pointToLight * ( i * stepSize);
            
        //     float noiseValue = texture(noiseTex, newPos * frequency).r;
        //     noiseValue = min(1, max(0, noiseValue - densityThreshold)) * densityFactor * 0.1;

        //     lightDensity += exp(-noiseValue * stepSize);
        // }
        // // lightDensity = exp(-lightDensity);
        //_________________________________

        return value;
    }

    //main
    void main()
    {   
        vec3 color = texture(albedoTexture, fragmentUv).rgb;
        vec2 clipSpaceCoords = fragmentUv * 2 - 1;

        vec4 rayOrig = camModelMatrix * vec4(0, 0, 0, 1);
        vec4 rayDir = vec4(0, 0, 1, 0);
        rayDir.x = abs(tan(camFov * 0.5)) * clipSpaceCoords.x * camAspectRatio;
        rayDir.y = abs(tan(camFov * 0.5)) * clipSpaceCoords.y;
        rayDir = camModelMatrix * rayDir;
        rayDir = normalize(rayDir);

        float distance=0;
        bool intersects = RayBoxTest(rayOrig.xyz, rayDir.xyz, boxTransform, vec3(-0.5),vec3(0.5), distance);
        
        vec3 finalColor = color;
        if(intersects) {
            vec3 hitPoint = rayOrig.xyz + rayDir.xyz * (distance + 0.0001);


            float insideDistance=0;
            RayBoxTest(hitPoint , rayDir.xyz, boxTransform, vec3(-0.5),vec3(0.5), insideDistance);

            float sampleSize = insideDistance - 0.2;
            float numSteps = 50.0f;
            float stepSize = sampleSize / numSteps;
 
            float finalValue = 0;
            float finalLightDensity=1;
            for(float i=2; i<numSteps; i++) {
                vec3 newPos = rayOrig.xyz + rayDir.xyz * ( distance + i * stepSize);
                
                float lightDensity=0;
                float noiseValue = sampleDensity(newPos, lightDensity);

                finalLightDensity += lightDensity * stepSize;
                finalValue += noiseValue * stepSize;
            }
            finalValue = exp(-finalValue);

            finalColor = finalValue * color  + (1 - finalValue) * vec3(finalLightDensity, finalLightDensity, finalLightDensity);
        }
        outputColor= vec4(finalColor.rgb, 1);
    }
    )";
    cloudShader.name = "cloudShader";
    cloudShader.SetId(6);
    cloudShader.Compile();
    
    quad = GetQuad(scene, "Cloud", glm::vec3(0), glm::vec3(0), glm::vec3(1), glm::vec4(1, 1, 1, 1));
    quadMaterial =  quad->GetComponent<MaterialComponent>();
    quadMaterial->SetShader(cloudShader);
    
    quad->Start(); 
    quad->Enable(); 

    //PERLIN 3D
    // std::vector<uint8_t> noiseTextureData(texRes * texRes * texRes);
    // for(int x=0, inx=0; x<texRes; x++) {
    //     for(int y=0; y<texRes; y++) {
    //         for(int z=0; z<texRes; z++, inx++) {
    //             glm::vec3 texCoord((float)x / (float)texRes, (float)y / (float)texRes, (float)z / (float)texRes);
    //             KikooRenderer::Util::NoiseSample sampleX =  KikooRenderer::Util::GetFracNoise3D(texCoord.x,texCoord.y,texCoord.z, 4, 2);
    //             uint8_t noiseVal = sampleX.value * 255;
    //             noiseTextureData[inx] = noiseVal;
    //         }            
    //     }            
    // }

    //CLOUD 3D
    int texRes = 128;
    std::vector<uint8_t> cloudTexture(texRes * texRes * texRes * 4);

    Util::ThreadPool( std::function<void(uint64_t, uint64_t)>([this, texRes, &cloudTexture](uint64_t i, uint64_t t)
    {
        uint64_t inx = i * 4;
        // uint64_t z = t % texRes * texRes;
        int z = inx / (texRes * texRes);
        uint64_t _inx = inx - (z * texRes * texRes);
        int y = _inx / texRes;
        int x = _inx % texRes;
        glm::vec3 uvw(((float)x / (float)texRes) , ((float)y / (float)texRes) , ((float)z / (float)texRes)  );
    
        float r = Util::GetPerlinWorleyNoise(uvw.x, uvw.y, uvw.z, 4);
        float g = Util::GetWorleyNoise3D(uvw.x, uvw.y, uvw.z, 1);
        float b = Util::GetWorleyNoise3D(uvw.x, uvw.y, uvw.z, 8);
        float a = Util::GetWorleyNoise3D(uvw.x, uvw.y, uvw.z, 16);
        cloudTexture[inx] =   (uint8_t)(r * 255);
        cloudTexture[inx+1] = (uint8_t)(g * 255);
        cloudTexture[inx+2] = (uint8_t)(b * 255);
        cloudTexture[inx+3] = (uint8_t)(a * 255);
    }), cloudTexture.size()/4).Block();
    noiseTex = Texture3D(1, cloudTexture, texRes, texRes,texRes, 4);


    

    // noiseTex = Texture3D(1, cloudTexture, texRes, texRes,texRes, 1);
}

void Cloud_1::WindowResize(int w, int h) {
}


void Cloud_1::Start() {
	started = true;
}

void Cloud_1::Enable() {
	enabled = true;
}

void Cloud_1::Update() {
    scene->triggerRefresh = true;
}

std::vector<QWidget*> Cloud_1::GetInspectorWidgets() {
    std::vector<QWidget*> res;
    res.push_back(this->transform->GetInspector());
    
    QGroupBox* mainGroupbox = new QGroupBox("Cloud");
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainGroupbox->setLayout(mainLayout);


    
    CustomSlider* densityThresholdSlider = new CustomSlider(0, 0.5, 0.001, "densityThreshold", densityThreshold);
    mainLayout->addLayout(densityThresholdSlider);
    QObject::connect( densityThresholdSlider, &CustomSlider::Modified, [this](double val) {
        densityThreshold = val;
        scene->triggerRefresh=true;
    });
    
        
    CustomSlider* densityFactorSlider = new CustomSlider(0, 500, 1, "densityFactor", densityFactor);
    mainLayout->addLayout(densityFactorSlider);
    QObject::connect( densityFactorSlider, &CustomSlider::Modified, [this](double val) {
        densityFactor = val;
        scene->triggerRefresh=true;
    });
     
    CustomSlider* frequencySlider = new CustomSlider(0, 4, 0.001, "frequency", frequency);
    mainLayout->addLayout(frequencySlider);
    QObject::connect( frequencySlider, &CustomSlider::Modified, [this](double val) {
        frequency = val;
        scene->triggerRefresh=true;
    });


    CustomSlider* channel0InfluenceSlider = new CustomSlider(0, 1, 0.001, "channel0Influence", channel0Influence);
    mainLayout->addLayout(channel0InfluenceSlider);
    QObject::connect( channel0InfluenceSlider, &CustomSlider::Modified, [this](double val) {
        channel0Influence = val;
        scene->triggerRefresh=true;
    });  
    
    CustomSlider* channel1InfluenceSlider = new CustomSlider(0, 1, 0.001, "channel1Influence", channel1Influence);
    mainLayout->addLayout(channel1InfluenceSlider);
    QObject::connect( channel1InfluenceSlider, &CustomSlider::Modified, [this](double val) {
        channel1Influence = val;
        scene->triggerRefresh=true;
    });  
    
    CustomSlider* channel2InfluenceSlider = new CustomSlider(0, 1, 0.001, "channel2Influence", channel2Influence);
    mainLayout->addLayout(channel2InfluenceSlider);
    QObject::connect( channel2InfluenceSlider, &CustomSlider::Modified, [this](double val) {
        channel2Influence = val;
        scene->triggerRefresh=true;
    });  
    
    CustomSlider* channel3InfluenceSlider = new CustomSlider(0, 1, 0.001, "channel3Influence", channel3Influence);
    mainLayout->addLayout(channel3InfluenceSlider);
    QObject::connect( channel3InfluenceSlider, &CustomSlider::Modified, [this](double val) {
        channel3Influence = val;
        scene->triggerRefresh=true;
    });    
    
    res.push_back(mainGroupbox);
    return res;
}


void Cloud_1::Render(glm::mat4* overrideViewMatrixp) {
    GETGL
    quad->Render();
}

void Cloud_1::RayMarch(Framebuffer* _fb) {
    GETGL
    if(!quadMaterial->albedoTex.loaded) {
        quadMaterial->albedoTex.glTex =  _fb->texture;
        quadMaterial->albedoTex.loaded = true;
        quadMaterial->albedoTex.texIndex = GL_TEXTURE0;
    }
    ogl->glUseProgram(quadMaterial->shader.programShaderObject);

    // ogl->glUniform1f(ogl->glGetUniformLocation(cloudShader.programShaderObject, "time"), scene->elapsedTime);
    

    ogl->glUniformMatrix4fv(ogl->glGetUniformLocation(cloudShader.programShaderObject, "camModelMatrix"), 1, false, glm::value_ptr(scene->camera->transform->GetWorldModelMatrix()));  
    
    ogl->glUniform1f(ogl->glGetUniformLocation(cloudShader.programShaderObject, "camFov"), scene->camera->GetFov());  
    ogl->glUniform1f(ogl->glGetUniformLocation(cloudShader.programShaderObject, "camAspectRatio"), scene->camera->GetAspect());  
    
    ogl->glUniform1f(ogl->glGetUniformLocation(cloudShader.programShaderObject, "densityThreshold"), densityThreshold);  
    ogl->glUniform1f(ogl->glGetUniformLocation(cloudShader.programShaderObject, "densityFactor"), densityFactor);  
    ogl->glUniform1f(ogl->glGetUniformLocation(cloudShader.programShaderObject, "frequency"), frequency);  

    ogl->glUniform1f(ogl->glGetUniformLocation(cloudShader.programShaderObject, "channel0Influence"), channel0Influence);  
    ogl->glUniform1f(ogl->glGetUniformLocation(cloudShader.programShaderObject, "channel1Influence"), channel1Influence);  
    ogl->glUniform1f(ogl->glGetUniformLocation(cloudShader.programShaderObject, "channel2Influence"), channel2Influence);  
    ogl->glUniform1f(ogl->glGetUniformLocation(cloudShader.programShaderObject, "channel3Influence"), channel3Influence);  

    ogl->glUniformMatrix4fv(ogl->glGetUniformLocation(cloudShader.programShaderObject, "boxTransform"), 1, false, glm::value_ptr(transform->GetWorldModelMatrix()));  

    ogl->glUniform3fv(ogl->glGetUniformLocation(cloudShader.programShaderObject, "boxScale"), 1,  glm::value_ptr(transform->scale));

    if(scene->lightObjects.size() > 0) {
        ogl->glUniform3fv(ogl->glGetUniformLocation(cloudShader.programShaderObject, "lightPos"), 1,  glm::value_ptr(scene->lightObjects[0]->transform->position));
    }
    

    ogl->glActiveTexture(GL_TEXTURE1);
    ogl->glBindTexture(GL_TEXTURE_3D, noiseTex.glTex);
    ogl->glUniform1i(ogl->glGetUniformLocation(cloudShader.programShaderObject, "noiseTex"), 1);  
    

    //In the shader 
    //step through the ray


    quad->Render();
    // _fb->Disable();    
}

void Cloud_1::DepthRenderPass(LightComponent* light) {
}

void Cloud_1::Destroy() {
}

void Cloud_1::Recompute() {
}


}
}