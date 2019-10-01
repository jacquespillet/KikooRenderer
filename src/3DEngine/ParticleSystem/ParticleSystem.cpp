#include "ParticleSystem.hpp"
#include "../Scene.hpp"
#include "../BaseObjects.hpp"
#include "../Texture.hpp"
#include "Util/RandomUtil.hpp"


#include "ParticleShader.hpp"
namespace KikooRenderer {

namespace CoreEngine {

void InsertionSort(std::vector<Particle>& arr)  
{  
    Particle key;
    int i, j;  
    for (i = 1; i < arr.size(); i++) 
    {  
        key = arr[i];  
        j = i - 1;  
  
        while (j >= 0 && arr[j].distance < key.distance) 
        {  
            arr[j + 1] = arr[j];  
            j = j - 1;  
        }  
        arr[j + 1] = key;  
    }  
} 


ParticleSystem::ParticleSystem(std::string name, Scene* scene) : Object3D(name, scene) {
    particleShader = GetParticleShader();
    
    quad = GetQuad(scene, "particle", glm::vec3(0), glm::vec3(0), glm::vec3(0.3), glm::vec4(0.5, 0.5, 0.5, 1));
    quad->Start();
    quad->Enable();
    quad->faceCamera = true;

    quadmaterial =  quad->GetComponent<MaterialComponent>();
    quadmaterial->SetShader(particleShader);

    Texture albedoTex = Texture(textureFile, GL_TEXTURE0);
    quadmaterial->albedoTex = albedoTex;
    numRows = 4;

    quadMeshFilter = quad->GetComponent<MeshFilterComponent>();
    quadMeshFilter->renderInstanced = true;

}


void ParticleSystem::Start() {
	started = true;
}

void ParticleSystem::Enable() {
	enabled = true;
}

void ParticleSystem::Render(glm::mat4* overrideViewMatrix) {
    GETGL
    ogl->glDepthMask(false);
    ogl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    

    ogl->glUseProgram(quadmaterial->shader.programShaderObject);
    ogl->glUniform1i(ogl->glGetUniformLocation(quadmaterial->shader.programShaderObject, "rowNum"), numRows);
    
    quadMeshFilter->numInstances = particles.size();
    std::vector<MeshFilterComponent::InstanceAttribute> instanceAttributes = std::vector<MeshFilterComponent::InstanceAttribute>(particles.size());
    for(int i=0; i<particles.size(); i++) {
        quad->transform->position = particles[i].position;
        quad->transform->scale = glm::vec3(particles[i].scale);
        quad->transform->rotation = glm::vec3(0, 0, particles[i].rotation);

		glm::mat4 vMatrix = scene->camera->GetViewMatrix();
		glm::mat4 pMatrix = scene->camera->GetProjectionMatrix();
        glm::mat4 mMatrix = glm::translate(glm::mat4(1.0), glm::vec3(quad->transform->position));
        mMatrix[0][0] = vMatrix[0][0]; mMatrix[1][0] = vMatrix[0][1]; mMatrix[2][0] = vMatrix[0][2];
        mMatrix[0][1] = vMatrix[1][0]; mMatrix[1][1] = vMatrix[1][1]; mMatrix[2][1] = vMatrix[1][2];
        mMatrix[0][2] = vMatrix[2][0]; mMatrix[1][2] = vMatrix[2][1]; mMatrix[2][2] = vMatrix[2][2];
        mMatrix = glm::rotate(mMatrix, (float)quad->transform->rotation.z * (float)DEGTORAD, glm::vec3(0.0f, 0.0f, 1.0f));
        mMatrix = glm::scale(mMatrix, glm::vec3(quad->transform->scale));

        glm::mat4 mvp = pMatrix * vMatrix *  mMatrix;
        
        instanceAttributes[i].modelMatrix = mvp;
        instanceAttributes[i].additionalData1 = glm::vec4(particles[i].offset1.x, particles[i].offset1.y, particles[i].offset2.x, particles[i].offset2.y);
        instanceAttributes[i].additionalData2 = glm::vec4(particles[i].blendValue, 0, 0, 0);
    }

    quadMeshFilter->SetInstanceAttributes(instanceAttributes);
    quad->Render();

    ogl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    
    ogl->glDepthMask(true);
}

void ParticleSystem::DepthRenderPass(LightComponent* light) {
}

void ParticleSystem::Update() {
    GenerateParticles();

    std::vector<Particle>::iterator it;
    for(it = particles.begin(); it != particles.end();)
    {
        bool alive = it->Update(scene->camera->transform->position);
        if(!alive) {
            it=particles.erase(it);
        } else ++it;
    }    
    InsertionSort(particles);
    scene->triggerRefresh = true;
}

void ParticleSystem::Destroy() {
    delete quad;
}

void ParticleSystem::Recompute() {
}

void ParticleSystem::GenerateParticles() {
    float delta = scene->deltaTime;
    if(delta * pps < 1.0) {
        toSpawn += delta * pps;
    } else {
        toSpawn = delta * pps;
    }    

    int spawnCount = (int) toSpawn;
    for(int i=0; i<spawnCount; i++) {
        EmitParticle();
    }

    if (delta * pps < 1.0 && toSpawn >= 1.0)
    {
        toSpawn = 0.0f;
    }    
}


void ParticleSystem::EmitParticle() {
    glm::vec3 velocity(0);
    if(useDirection){
        velocity = Util::GenerateRandomUnitVectorWithinCone(transform->rotation, directionDeviation);
    }else{
        velocity = Util::GenerateRandomUnitVector();
    }
    glm::normalize(velocity);
    velocity *= Util::GenerateValue(speed, speedError);

    float finalScale = Util::GenerateValue(scale, scaleError);
    float finalLifeLength = Util::GenerateValue(lifeLength, lifeError);
    float finalRotation = isRandomRotation ? Util::GetRand() * 360.0f : 0;

    Particle particle(scene, numRows, this->transform->position, velocity, gravityFactor, finalLifeLength, finalRotation, finalScale, useCurlNoise);   
    particles.push_back(particle); 
}

QWidget* ParticleSystem::GetMainParameters() {
    QGroupBox* mainGroupbox = new QGroupBox("Particle System");
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainGroupbox->setLayout(mainLayout);

    CustomSlider* ppsSlider = new CustomSlider(1, 10000, 1, "Particles Per Second", pps);
    mainLayout->addLayout(ppsSlider);
    QObject::connect( ppsSlider, &CustomSlider::Modified, [this](double val) {
        pps = val;
    });


    CustomSlider* speedSlider = new CustomSlider(1, 50, 0.1, "Speed", speed);
    mainLayout->addLayout(speedSlider);
    QObject::connect( speedSlider, &CustomSlider::Modified, [this](double val) {
        speed = val;
    });    


    CustomSlider* scaleSlider = new CustomSlider(1, 30, 0.1, "scale", scale);
    mainLayout->addLayout(scaleSlider);
    QObject::connect( scaleSlider, &CustomSlider::Modified, [this](double val) {
        scale = val;
    });        

    CustomSlider* lifeLengthSlider = new CustomSlider(1, 100, 0.1, "lifeLength", lifeLength);
    mainLayout->addLayout(lifeLengthSlider);
    QObject::connect( lifeLengthSlider, &CustomSlider::Modified, [this](double val) {
        lifeLength = val;
    });  

    CustomSlider* gravityEffectSlider = new CustomSlider(1, 1, 0.01, "gravityEffect", gravityFactor);
    mainLayout->addLayout(gravityEffectSlider);
    QObject::connect( gravityEffectSlider, &CustomSlider::Modified, [this](double val) {
        gravityFactor = val;
    });  

    CustomSlider* speedErrorSlider = new CustomSlider(0, 5, 0.01, "speedError", speedError);
    mainLayout->addLayout(speedErrorSlider);
    QObject::connect( speedErrorSlider, &CustomSlider::Modified, [this](double val) {
        speedError = val;
    });  

    CustomSlider* lifeErrorSlider = new CustomSlider(0, 5, 0.01, "lifeError", lifeError);
    mainLayout->addLayout(lifeErrorSlider);
    QObject::connect( lifeErrorSlider, &CustomSlider::Modified, [this](double val) {
        lifeError = val;
    });  
    
    CustomSlider* scaleErrorSlider = new CustomSlider(0, 5, 0.01, "scaleError", scaleError);
    mainLayout->addLayout(scaleErrorSlider);
    QObject::connect( scaleErrorSlider, &CustomSlider::Modified, [this](double val) {
        scaleError = val;
    });  

    QCheckBox* randomRotCheckbox = new QCheckBox("Random Rotation");
    mainLayout->addWidget(randomRotCheckbox);
    QObject::connect( randomRotCheckbox, &QCheckBox::stateChanged, [this](int state) {
        isRandomRotation = state>0;
    });

    //Texture map
    FilePicker* texturePicker = new FilePicker("texture Atlas", textureFile);
    mainLayout->addWidget(texturePicker);
    QObject::connect(texturePicker, &FilePicker::FileModified, [this](QString string) {
        textureFile = string.toStdString();
        scene->glWindow->makeCurrent();
        Texture albedoTex = Texture(textureFile, GL_TEXTURE0);
        quadmaterial->albedoTex = albedoTex;        
        scene->glWindow->doneCurrent();
    });
    
    CustomSlider* numRowsSlider = new CustomSlider(0, 100, 1, "numRows", numRows);
    mainLayout->addLayout(numRowsSlider);
    QObject::connect( numRowsSlider, &CustomSlider::Modified, [this](double val) {
        numRows = val;
    });

    return mainGroupbox;
}

std::vector<QWidget*> ParticleSystem::GetInspectorWidgets() {
    std::vector<QWidget*> res;
    res.push_back(GetMainParameters());
	return res;
}



}
}