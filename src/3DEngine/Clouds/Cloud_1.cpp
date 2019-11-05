#include "Cloud_1.hpp"

#include "../BaseObjects.hpp"
#include "../Components/MaterialComponent.hpp"
#include "../Components/MeshFilterComponent.hpp"

#include "../Texture.hpp"
#include "Util/RandomUtil.hpp"
#include "Util/NoiseUtil.hpp"
#include "Util/Image.hpp"

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

    float RayBoxTest(vec3 rayOrig, vec3 rayDir, mat4 transform, vec3 minScale,vec3 maxScale)
    {
        float distance = -1;
        //transform ray
        mat4 invTr = inverse(transform); 
        rayOrig = vec3( invTr * vec4(rayOrig, 1) );
        rayDir = vec3( invTr * vec4(rayDir, 0) );
        
        //AABB Ray intersection 
        vec3 rayDirInv = vec3(1.0f / rayDir.x, 1.0f / rayDir.y, 1.0f / rayDir.z); 
        vec3 t1 = (minScale - rayOrig) * rayDirInv;
        vec3 t2 = (maxScale - rayOrig) * rayDirInv;
        
        vec3 intersectionPosition = rayOrig + t2.x * rayDir;
        float tmin = max( max( min(t1.x, t2.x), min(t1.y, t2.y)), min(t1.z, t2.z));
        float tmax = min( min( max(t1.x, t2.x), max(t1.y, t2.y)), max(t1.z, t2.z));
        bool raybox = tmax >= max(0.0f, tmin);
        distance = tmin; 
    
        if(!raybox)
            distance = -1;

        return distance;
    }

    float sampleDensity(vec3 position) {
        // float uvw = position * offset + offset; 
        float value = texture(noiseTex, position * frequency).r;
        value = min(1, max(0, value - densityThreshold)) * densityFactor;
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

        float distance = RayBoxTest(rayOrig.xyz, rayDir.xyz, boxTransform, vec3(-0.5),vec3(0.5));
        
        vec3 finalColor = color;
        if(distance > 0) {
            // vec3 hitPoint = rayOrig.xyz + rayDir.xyz * distance;
            // float value = texture(noiseTex, vec3(hitPoint.x, hitPoint.y, (time * 0.25))).r;
            // finalColor = vec3(value, value, value);

            float sampleSize = 1;
            float numSteps = 10.0f;
            float stepSize = (sampleSize / numSteps);

            float finalValue = 0;
            for(float i=0; i<numSteps; i++) {
                vec3 newPos = rayOrig.xyz + rayDir.xyz * ( distance + i * stepSize);
                
                float noiseValue = sampleDensity(newPos);
                finalValue += noiseValue * stepSize;
            }
            finalValue = exp(-finalValue);

            finalColor = finalValue * color  + (1 - finalValue) * vec3(1, 1, 1);
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
    std::vector<uint8_t> cloudTexture(texRes * texRes * texRes);

    for(int z=0, inx=0; z< texRes; z++) {
        for(int y=0; y< texRes; y++) {
            for(int x=0; x< texRes; x++, inx++) {
                if(inx % 10000 == 0) std::cout << inx << std::endl;
                glm::vec3 uvw(((float)x / (float)texRes) , ((float)y / (float)texRes) , ((float)z / (float)texRes)  );
                float r = Util::GetPerlinWorleyNoise(uvw.x, uvw.y, uvw.z, 4);
                cloudTexture[inx] = (uint8_t)(r * 255);
            }
        }
    }


    
    noiseTex = Texture3D(1, cloudTexture, texRes, texRes,texRes, 1);
    
        

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


    
    CustomSlider* densityThresholdSlider = new CustomSlider(0, 0.1, 0.0001, "densityThreshold", densityThreshold);
    mainLayout->addLayout(densityThresholdSlider);
    QObject::connect( densityThresholdSlider, &CustomSlider::Modified, [this](double val) {
        densityThreshold = val;
        scene->triggerRefresh=true;
    });
    
        
    CustomSlider* densityFactorSlider = new CustomSlider(0, 100, 0.5, "densityFactor", densityFactor);
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

    ogl->glUniformMatrix4fv(ogl->glGetUniformLocation(cloudShader.programShaderObject, "boxTransform"), 1, false, glm::value_ptr(transform->GetWorldModelMatrix()));  

    ogl->glUniform3fv(ogl->glGetUniformLocation(cloudShader.programShaderObject, "boxScale"), 1,  glm::value_ptr(transform->scale));

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