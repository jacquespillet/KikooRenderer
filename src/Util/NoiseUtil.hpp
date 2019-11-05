#pragma once

#include "Util/Common.h"

namespace KikooRenderer {
namespace Util {

struct NoiseSample {
    float value;
    glm::vec3 derivative;
};

//Value Noise
NoiseSample GetValueNoise1D(float x, float frequency = 256);

NoiseSample GetValueNoise2D(float x, float y,float frequency = 256); 

NoiseSample GetValueNoise3D(float x, float y, float t, float frequency = 256);   

//Perlin Noise  
NoiseSample GetPerlinNoise1D(float x, float frequency = 256);

NoiseSample GetPerlinNoise2D(float x, float y,float frequency = 256); 

NoiseSample GetPerlinNoise3D(float x, float y, float t, float frequency = 256);   

//Fractal Noise  
NoiseSample GetFracNoise1D(float x, float frequency = 16, int octaves = 2, float lacunarity = 2.0f, float persistence = 0.5f);

NoiseSample GetFracNoise2D(float x, float y,float frequency = 16, int octaves = 2, float lacunarity = 2.0f, float persistence = 0.5f); 

NoiseSample GetFracNoise3D(float x, float y, float t, float frequency = 16, int octaves = 2, float lacunarity = 2.0f, float persistence = 0.5f);   

//Worley Noise
float GetWorleyNoise3D(float x, float y, float z, float frequency);


float GetPerlinWorleyNoise(float x, float y, float z,float frequency);


}
}