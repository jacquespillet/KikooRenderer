#include "Cloud_1.hpp"

#include "../BaseObjects.hpp"
#include "../Components/MaterialComponent.hpp"
#include "../Components/MeshFilterComponent.hpp"

#include "../Texture.hpp"
#include "Util/RandomUtil.hpp"
#include "Util/NoiseUtil.hpp"

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
        float value = texture(noiseTex, position).r;
        // value = max(0, value - 0.2) * densityMultiplier;
        value = max(0, value - 0.2) * 10;
        // value *= 10;
        
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
            float sampleSize = 1;
            float numSteps = 100.0f;
            float stepSize = (sampleSize / numSteps) * 1 ;

            float finalValue = 0;
            for(float i=0; i<numSteps; i++) {
                vec3 newPos = rayOrig.xyz + rayDir.xyz * ( distance + i * stepSize);

                float noiseValue = sampleDensity(newPos);
                finalValue += noiseValue * stepSize;
            }
            finalValue = exp(-finalValue);
            finalValue = max(0, finalValue - 0.1);

            finalColor = finalValue * vec3(1, 1, 1) + (1 - finalValue) * color;
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
    uint32_t numPoints = 25;
    std::vector<glm::vec3> points(numPoints);

    for(int i=0; i<numPoints; i++) {
        points[i] = glm::vec3((Util::GetRand() * texRes), (Util::GetRand() * texRes), (Util::GetRand() * texRes));
    }

    points.resize(numPoints * 2);
    for(int i=0, j=numPoints; i<numPoints; i++, j++) {
        points[j] = points[i] + glm::vec3(0, 0, texRes);    
    }
    
    points.resize(numPoints * 3);
    for(int i=0, j=numPoints * 2; i<numPoints; i++, j++) {
        points[j] = points[i] + glm::vec3(0, 0, -texRes);
    }

    points.resize(numPoints * 4);
    for(int i=0, j=numPoints * 3; i<numPoints; i++, j++) {
        points[j] = points[i] + glm::vec3(texRes, 0, 0);
    }

    points.resize(numPoints * 5);
    for(int i=0, j=numPoints * 4; i<numPoints; i++, j++) {
        points[j] = points[i] + glm::vec3(-texRes, 0, 0);
    }


    points.resize(numPoints * 5);
    for(int i=0, j=numPoints * 4; i<numPoints; i++, j++) {
        points[j] = points[i] + glm::vec3(0, texRes, 0);
    }

    points.resize(numPoints * 6);
    for(int i=0, j=numPoints * 5; i<numPoints; i++, j++) {
        points[j] = points[i] + glm::vec3(0, -texRes, 0);
    }

    std::vector<uint8_t> cloudTexture(texRes * texRes * texRes);
    
    // for(int i=0; i< texRes * texRes* texRes; i++) {
    float maxDistance = -1;
    for(int z=0, inx=0; z< texRes; z++) {
        for(int y=0; y< texRes; y++) {
            for(int x=0; x< texRes; x++, inx++) {
                float distance = texRes * texRes;
                glm::vec3 pos((float)x, (float)y, (float)z);

                for(int j=0; j<points.size(); j++) {
                    float currentDistance = glm::distance(pos, points[j]);
                    if(currentDistance < distance) distance = currentDistance;
                }
                cloudTexture[inx] = distance;

                if(distance > maxDistance) maxDistance = distance;
            }
        }
    }

    for(int i=0; i<cloudTexture.size(); i++) {
        cloudTexture[i] = ((float)cloudTexture[i] / maxDistance) * 255;
    }
    
    noiseTex = Texture3D(1, cloudTexture, texRes, texRes,texRes, 1);
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