#include "Particle.hpp"
#include "../Scene.hpp"

namespace KikooRenderer {

namespace CoreEngine {
Particle::Particle(Scene* scene, int numRows, glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale) {
    this->scene =scene;
    this->position =position;
    this->velocity =velocity;
    this->gravityEffect =gravityEffect;
    this->lifeLength =lifeLength;
    this->rotation =rotation;
    this->scale =scale;
    this->numRows =numRows;
}

bool Particle::Update(glm::vec3 camPos) {
    UpdateTexCoords();

    velocity.y += gravityEffect * -9.81 * scene->deltaTime;

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