#include "Particle.hpp"
#include "../Scene.hpp"

#include "Util/NoiseUtil.hpp"
#include "Util/RandomUtil.hpp"
namespace KikooRenderer {

namespace CoreEngine {
Particle::Particle(Scene* scene, int numRows, glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale, bool useCurlNoise) {
    this->scene =scene;
    this->position =position;
    // this->velocity =velocity;
    this->gravityEffect =gravityEffect;
    this->lifeLength =lifeLength;
    this->rotation =rotation;
    this->scale =scale;
    this->numRows =numRows;
    this->useCurlNoise = useCurlNoise;
}

bool Particle::Update(glm::vec3 camPos) {
    UpdateTexCoords();

    if(!useCurlNoise) {
        velocity.y += gravityEffect * -9.81 * scene->deltaTime;
    } else {
        // KikooRenderer::Util::NoiseSample sample = KikooRenderer::Util::GetFracNoise3D(position.x,position.y,position.z, 2, 6, 2.0, 0.5);        
        // glm::vec3 offset = 0.1f * glm::vec3(Util::GetRand(), Util::GetRand(), Util::GetRand());
        float amplitude = 1;

        glm::vec3 offset = 0.3f * glm::vec3(Util::GetRand() * 2.0 - 1.0, Util::GetRand() * 2.0 - 1.0, Util::GetRand() * 2.0 - 1.0);
        glm::vec3 point = glm::vec3(position.z, position.y, position.x) + offset;
        KikooRenderer::Util::NoiseSample sampleX =  KikooRenderer::Util::GetFracNoise3D(point.x,point.y,point.z, 0.05, 4);
        sampleX.value *= amplitude;
        sampleX.derivative *= amplitude;

        offset = 0.3f * glm::vec3(Util::GetRand() * 2.0 - 1.0, Util::GetRand() * 2.0 - 1.0, Util::GetRand() * 2.0 - 1.0);
        point = glm::vec3(position.x + 100.0f, position.z, position.y) + offset;
        KikooRenderer::Util::NoiseSample sampleY =  KikooRenderer::Util::GetFracNoise3D(point.x,point.y,point.z, 0.05, 4);
        sampleY.value *= amplitude;
        sampleY.derivative *= amplitude;

        offset = 0.3f * glm::vec3(Util::GetRand() * 2.0 - 1.0, Util::GetRand() * 2.0 - 1.0, Util::GetRand() * 2.0 - 1.0);
        point = glm::vec3(position.y, position.x + 100.0f, position.z) + offset;
        KikooRenderer::Util::NoiseSample sampleZ =  KikooRenderer::Util::GetFracNoise3D(point.x,point.y,point.z, 0.05, 4);
        sampleZ.value *= amplitude;
        sampleZ.derivative *= amplitude;

        glm::vec3 curl;
        curl.x = sampleZ.derivative.x - sampleY.derivative.y;
        curl.y = sampleX.derivative.x - sampleZ.derivative.y;
        curl.z = sampleY.derivative.x - sampleX.derivative.y;              
        
        velocity = 4.0f * glm::normalize(curl);
        

    }

    glm::vec3 change(velocity);
    change *= scene->deltaTime;
    
    position += change;

    distance = glm::distance(camPos, position);

    elapsedTime += scene->deltaTime;

    return elapsedTime < lifeLength;
}

void Particle::UpdateTexCoords() {
    float lifeFactor = elapsedTime / (float)lifeLength;
    int stageCount = numRows * numRows;

    float atlasProgression = lifeFactor * (float) stageCount;

    int index = (int)std::floor(atlasProgression);
    int nextIndex = index < stageCount ? index +1 : index;
    
    blendValue = atlasProgression - (int)atlasProgression;

    int column = index % numRows;
    int row =    std::floor((float)index / (float)numRows);
    offset1.x = (float) column / (float)numRows;
    offset1.y = 1 - (float) row / (float)numRows - (1.0 / (float)numRows);
    
    column = nextIndex % numRows;
    row =    nextIndex / numRows;
    offset2.x = (float) column /(float) numRows;
    offset2.y = 1 - (float) row / (float)numRows - (1.0 / (float)numRows);
}

}
}