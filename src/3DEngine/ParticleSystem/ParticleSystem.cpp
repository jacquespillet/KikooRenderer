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
        glm::vec3 direction = glm::column(transform->GetWorldModelMatrix(), 2);
        velocity = Util::GenerateRandomUnitVectorWithinCone(direction, directionDeviation);
    }else{
        velocity = Util::GenerateRandomUnitVector();
    }
    glm::normalize(velocity);
    velocity *= Util::GenerateValue(speed, speedError);

    float finalScale = Util::GenerateValue(scale, scaleError);
    float finalLifeLength = Util::GenerateValue(lifeLength, lifeError);
    float finalRotation = isRandomRotation ? Util::GetRand() * 360.0f : 0;

    Particle particle(scene, numRows, this->transform->position, velocity, gravityFactor, finalLifeLength, finalRotation, finalScale,this, useCurlNoise);   
    particles.push_back(particle); 
}

QWidget* ParticleSystem::GetMainParameters() {
    QGroupBox* mainGroupbox = new QGroupBox("Particle System");
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainGroupbox->setLayout(mainLayout);

    CustomSlider* ppsSlider = new CustomSlider(1, 10000, 10, "Particles Per Second", pps);
    mainLayout->addLayout(ppsSlider);
    QObject::connect( ppsSlider, &CustomSlider::Modified, [this](double val) {
        pps = val;
    });


    CustomSlider* speedSlider = new CustomSlider(1, 50, 0.5, "Speed", speed);
    mainLayout->addLayout(speedSlider);
    QObject::connect( speedSlider, &CustomSlider::Modified, [this](double val) {
        speed = val;
    });    


    CustomSlider* scaleSlider = new CustomSlider(0.1, 15, 0.1, "scale", scale);
    mainLayout->addLayout(scaleSlider);
    QObject::connect( scaleSlider, &CustomSlider::Modified, [this](double val) {
        scale = val;
    });        

    CustomSlider* lifeLengthSlider = new CustomSlider(1, 60, 0.1, "lifeLength", lifeLength);
    mainLayout->addLayout(lifeLengthSlider);
    QObject::connect( lifeLengthSlider, &CustomSlider::Modified, [this](double val) {
        lifeLength = val;
    });  

    CustomSlider* gravityEffectSlider = new CustomSlider(0, 1, 0.01, "gravityEffect", gravityFactor);
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
    TexturePicker* texturePicker = new TexturePicker("texture Atlas", textureFile);
    mainLayout->addWidget(texturePicker);
    QObject::connect(texturePicker, &TexturePicker::FileModified, [this](QString string) {
        textureFile = string.toStdString();
        scene->glWindow->makeCurrent();
        quadmaterial->SetAlbedoTex(Texture(textureFile, GL_TEXTURE0));		
        scene->glWindow->doneCurrent();
    });
    
    CustomSlider* numRowsSlider = new CustomSlider(0, 100, 1, "numRows", numRows);
    mainLayout->addLayout(numRowsSlider);
    QObject::connect( numRowsSlider, &CustomSlider::Modified, [this](double val) {
        numRows = val;
    });

    
    QCheckBox* curlNoiseCheckbox = new QCheckBox("Use Curl Noise");
    mainLayout->addWidget(curlNoiseCheckbox);

    QGroupBox* noiseGroupBox = new QGroupBox("Noise Params");
    QVBoxLayout* noiseMainLayout = new QVBoxLayout();
    noiseGroupBox->setLayout(noiseMainLayout);
    noiseGroupBox->setVisible(useCurlNoise);


    CustomSlider* frequencyXSlider = new CustomSlider(0, 5, 0.01, "frequency X", frequencyX);
    noiseMainLayout->addLayout(frequencyXSlider);
    QObject::connect( frequencyXSlider, &CustomSlider::Modified, [this](double val) {
        frequencyX = val;
    });

    CustomSlider* frequencyYSlider = new CustomSlider(0, 5, 0.01, "frequency Y", frequencyY);
    noiseMainLayout->addLayout(frequencyYSlider);
    QObject::connect( frequencyYSlider, &CustomSlider::Modified, [this](double val) {
        frequencyY = val;
    });
    
    CustomSlider* frequencyZSlider = new CustomSlider(0, 5, 0.01, "frequency Z", frequencyZ);
    noiseMainLayout->addLayout(frequencyZSlider);
    QObject::connect( frequencyZSlider, &CustomSlider::Modified, [this](double val) {
        frequencyZ = val;
    });

  
    CustomSlider* octavesXSlider = new CustomSlider(0, 8, 1, "octaves X", octavesX);
    noiseMainLayout->addLayout(octavesXSlider);
    QObject::connect( octavesXSlider, &CustomSlider::Modified, [this](double val) {
        octavesX = val;
    });

    CustomSlider* octavesYSlider = new CustomSlider(0, 8, 1, "octaves Y", octavesY);
    noiseMainLayout->addLayout(octavesYSlider);
    QObject::connect( octavesYSlider, &CustomSlider::Modified, [this](double val) {
        octavesY = val;
    });
    
    CustomSlider* octavesZSlider = new CustomSlider(0, 8, 1, "octaves Z", octavesZ);
    noiseMainLayout->addLayout(octavesZSlider);
    QObject::connect( octavesZSlider, &CustomSlider::Modified, [this](double val) {
        octavesZ = val;
    });    

    CustomSlider* amplitudeSlider = new CustomSlider(0, 3, 0.01, "amplitude", amplitude);
    noiseMainLayout->addLayout(amplitudeSlider);
    QObject::connect( amplitudeSlider, &CustomSlider::Modified, [this](double val) {
        amplitude = val;
    });    

    CustomSlider* offsetMultSlider = new CustomSlider(0, 1, 0.001, "offset Multiplier", offsetMult);
    noiseMainLayout->addLayout(offsetMultSlider);
    QObject::connect( offsetMultSlider, &CustomSlider::Modified, [this](double val) {
        offsetMult = val;
    });   

    CustomSlider* speedMultSlider = new CustomSlider(0, 10, 0.01, "speed Multiplier", speedMult);
    noiseMainLayout->addLayout(speedMultSlider);
    QObject::connect( speedMultSlider, &CustomSlider::Modified, [this](double val) {
        speedMult = val;
    });   

    mainLayout->addWidget(noiseGroupBox);

    QObject::connect( curlNoiseCheckbox, &QCheckBox::stateChanged, [this, noiseGroupBox](int state) {
        noiseGroupBox->setVisible(state > 0);
        useCurlNoise = state>0;
    });

    return mainGroupbox;
}

std::vector<QWidget*> ParticleSystem::GetInspectorWidgets() {
    std::vector<QWidget*> res;
    res.push_back(this->transform->GetInspector());
    res.push_back(GetMainParameters());
	return res;
}


float ParticleSystem::GetAmplitude() {
    return amplitude;
}

void ParticleSystem::SetAmplitude(float _amplitude) {
    this->amplitude = _amplitude;
}

float ParticleSystem::GetFrequencyX() {
    return frequencyX;
}

void ParticleSystem::SetFrequencyX(float _frequencyX) {
    this->frequencyX = _frequencyX;
}

float ParticleSystem::GetFrequencyY() {
    return frequencyY;
}

void ParticleSystem::SetFrequencyY(float _frequencyY) {
    this->frequencyY = _frequencyY;
}

float ParticleSystem::GetFrequencyZ() {
    return frequencyZ;
}

void ParticleSystem::SetFrequencyZ(float _frequencyZ) {
    this->frequencyZ = _frequencyZ;
}

int ParticleSystem::GetOctavesX() {
    return octavesX;
}

void ParticleSystem::SetOctavesX(int _octavesX) {
    this->octavesX = _octavesX;
}

int ParticleSystem::GetOctavesY() {
    return octavesY;
}

void ParticleSystem::SetOctavesY(int _octavesY) {
    this->octavesY = _octavesY;
}

int ParticleSystem::GetOctavesZ() {
    return octavesZ;
}

void ParticleSystem::SetOctavesZ(int _octavesZ) {
    this->octavesZ = _octavesZ;
}

float ParticleSystem::GetOffsetMult() {
    return offsetMult;
}

void ParticleSystem::SetOffsetMult(float _offsetMult) {
    this->offsetMult = _offsetMult;
}

float ParticleSystem::GetSpeedMult() {
    return speedMult;
}

void ParticleSystem::SetSpeedMult(float _speedMult) {
    this->speedMult = _speedMult;
}



}
}