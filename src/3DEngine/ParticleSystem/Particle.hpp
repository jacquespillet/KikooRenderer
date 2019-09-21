#pragma once
#include "Util/Common.h"

namespace KikooRenderer {

namespace CoreEngine {
class Scene;
class Particle {
public: 
    Particle(){}
    Particle(Scene* scene, glm::vec3 position, glm::vec3 velocity, float gravityEffect, float lifeLength, float rotation, float scale);

    bool Update(glm::vec3 camPos);
    
    glm::vec3 position;
    float scale;
    float rotation;

    float distance;


    //Tmp 
    int numRows=4;
    glm::vec2 offset1;
    glm::vec2 offset2;
    float blendValue;

private:
    Scene* scene;

    glm::vec3 velocity;
    float gravityEffect;
    float lifeLength;

    float elapsedTime=0;

    void UpdateTexCoords();

};

}
}