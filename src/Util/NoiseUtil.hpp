#pragma once

#include "Util/Common.h"

namespace KikooRenderer {
namespace Util {

//Value Noise
float GetValueNoise1D(float x, int frequency = 256);

float GetValueNoise2D(float x, float y,int frequency = 256); 

float GetValueNoise3D(float x, float y, float t, int frequency = 256);   

//Perlin Noise  
float GetPerlinNoise1D(float x, int frequency = 256);

float GetPerlinNoise2D(float x, float y,int frequency = 256); 

float GetPerlinNoise3D(float x, float y, float t, int frequency = 256);   

//Fractal Noise  
float GetFracNoise1D(float x, int frequency = 16, int octaves = 2, float lacunarity = 2.0f, float persistence = 0.5f);

float GetFracNoise2D(float x, float y,int frequency = 16, int octaves = 2, float lacunarity = 2.0f, float persistence = 0.5f); 

float GetFracNoise3D(float x, float y, float t, int frequency = 16, int octaves = 2, float lacunarity = 2.0f, float persistence = 0.5f);   

}
}