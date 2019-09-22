#include "NoiseUtil.hpp"


namespace KikooRenderer {
namespace Util {

float Lerp(float x, float y, float t) {
    return x * (1 - t) + y * t;
}
float Smooth (float t) {
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

int hash[512] = {
		151,160,137, 91, 90, 15,131, 13,201, 95, 96, 53,194,233,  7,225,
		140, 36,103, 30, 69,142,  8, 99, 37,240, 21, 10, 23,190,  6,148,
		247,120,234, 75,  0, 26,197, 62, 94,252,219,203,117, 35, 11, 32,
		 57,177, 33, 88,237,149, 56, 87,174, 20,125,136,171,168, 68,175,
		 74,165, 71,134,139, 48, 27,166, 77,146,158,231, 83,111,229,122,
		 60,211,133,230,220,105, 92, 41, 55, 46,245, 40,244,102,143, 54,
		 65, 25, 63,161,  1,216, 80, 73,209, 76,132,187,208, 89, 18,169,
		200,196,135,130,116,188,159, 86,164,100,109,198,173,186,  3, 64,
		 52,217,226,250,124,123,  5,202, 38,147,118,126,255, 82, 85,212,
		207,206, 59,227, 47, 16, 58, 17,182,189, 28, 42,223,183,170,213,
		119,248,152,  2, 44,154,163, 70,221,153,101,155,167, 43,172,  9,
		129, 22, 39,253, 19, 98,108,110, 79,113,224,232,178,185,112,104,
		218,246, 97,228,251, 34,242,193,238,210,144, 12,191,179,162,241,
		 81, 51,145,235,249, 14,239,107, 49,192,214, 31,181,199,106,157,
		184, 84,204,176,115,121, 50, 45,127,  4,150,254,138,236,205, 93,
		222,114, 67, 29, 24, 72,243,141,128,195, 78, 66,215, 61,156,180,

		151,160,137, 91, 90, 15,131, 13,201, 95, 96, 53,194,233,  7,225,
		140, 36,103, 30, 69,142,  8, 99, 37,240, 21, 10, 23,190,  6,148,
		247,120,234, 75,  0, 26,197, 62, 94,252,219,203,117, 35, 11, 32,
		 57,177, 33, 88,237,149, 56, 87,174, 20,125,136,171,168, 68,175,
		 74,165, 71,134,139, 48, 27,166, 77,146,158,231, 83,111,229,122,
		 60,211,133,230,220,105, 92, 41, 55, 46,245, 40,244,102,143, 54,
		 65, 25, 63,161,  1,216, 80, 73,209, 76,132,187,208, 89, 18,169,
		200,196,135,130,116,188,159, 86,164,100,109,198,173,186,  3, 64,
		 52,217,226,250,124,123,  5,202, 38,147,118,126,255, 82, 85,212,
		207,206, 59,227, 47, 16, 58, 17,182,189, 28, 42,223,183,170,213,
		119,248,152,  2, 44,154,163, 70,221,153,101,155,167, 43,172,  9,
		129, 22, 39,253, 19, 98,108,110, 79,113,224,232,178,185,112,104,
		218,246, 97,228,251, 34,242,193,238,210,144, 12,191,179,162,241,
		 81, 51,145,235,249, 14,239,107, 49,192,214, 31,181,199,106,157,
		184, 84,204,176,115,121, 50, 45,127,  4,150,254,138,236,205, 93,
		222,114, 67, 29, 24, 72,243,141,128,195, 78, 66,215, 61,156,180
};
int hashMask = 255;

float GetValueNoise1D(float x, int frequency) {
    x *= frequency;

    int i0 = (int)std::floor(x);
    float t = x - i0;

    i0 &= hashMask;
    int i1 = i0 + 1;

    int h0 = hash[i0];
    int h1 = hash[i1];
    
    t = Smooth(t);
    // return hash[i0] / (float)hashMask;
    return Lerp(h0, h1, t) / (float)hashMask;
}


float GetValueNoise2D(float x, float y, int frequency) {
    x *= frequency;
    y *= frequency;

    //Integer part
    int ix0 = (int)std::floor(x);
    int iy0 = (int)std::floor(y);
    
    //Opposite corners
    int ix1 = ix0 + 1;
    int iy1 = iy0 + 1;
    
    //Interpolators between bottomLeft and topRight
    float tx = x - ix0;
    float ty = y - iy0;
    tx = Smooth(tx);
    ty = Smooth(ty);

    //Make sure not exceeding size of mask
    ix0 &= hashMask;
    iy0 &= hashMask;

    //First hash of two corners
    int h0 = hash[ix0];
    int h1 = hash[ix1];

    //Secondary hashes (2D)
    int h10 = hash[h1 + iy0];
    int h01 = hash[h0 + iy1];
    int h00 = hash[h0 + iy0];
    int h11 = hash[h1 + iy1];

    return  Lerp( Lerp(h00, h10, tx), Lerp(h01, h11, tx), ty) / (float)hashMask;
}

float GetValueNoise3D(float x, float y, float t, int frequency) {
    x *= frequency;
    y *= frequency;
    t *= frequency;

    int ix0 = (int)std::floor(x);
    int iy0 = (int)std::floor(y);
    int it0 = (int)std::floor(t);

    float tx = x - ix0;
    float ty = y - iy0;
    float tt = t - it0;    
    tx = Smooth(tx);
    ty = Smooth(ty);
    tt = Smooth(tt);  

    ix0 &= hashMask;
    iy0 &= hashMask;
    it0 &= hashMask;

    int ix1 = ix0 + 1;
    int iy1 = iy0 + 1;
    int it1 = it0 + 1;

    int h0 = hash[ix0];
    int h1 = hash[ix1];
    int h00 = hash[h0 + iy0];
    int h10 = hash[h1 + iy0];
    int h01 = hash[h0 + iy1];
    int h11 = hash[h1 + iy1];
    int h000 = hash[h00 + it0];
    int h100 = hash[h10 + it0];
    int h010 = hash[h01 + it0];
    int h110 = hash[h11 + it0];
    int h001 = hash[h00 + it1];
    int h101 = hash[h10 + it1];
    int h011 = hash[h01 + it1];
    int h111 = hash[h11 + it1];  
    

    return Lerp(
        Lerp(Lerp(h000, h100, tx), Lerp(h010, h110, tx), ty),
        Lerp(Lerp(h001, h101, tx), Lerp(h011, h111, tx), ty),
        tt) * (1.0f / hashMask);
}


float gradients1D[2] = {
		-1, 1
};
int gradientMask1D = 1;

float GetPerlinNoise1D(float x, int frequency) {
    x *= frequency;
    int i0 = (int)std::floor(x);
    float t0 = x - i0;
    float t1 = t0 - 1.0f;
    
    i0 &= hashMask;
    int i1 = i0 + 1;
    
    float g0 = gradients1D[hash[i0] & gradientMask1D];
    float g1 = gradients1D[hash[i1] & gradientMask1D];

    float v0 = g0 * t0;
    float v1 = g1 * t1;

    float t = Smooth(t0);
    return Lerp(v0, v1, t) * 2;
}


glm::vec2 gradients2D[8] = {
    glm::vec2( 1.0f, 0.0f),
    glm::vec2(-1.0f, 0.0f),
    glm::vec2( 0.0f, 1.0f),
    glm::vec2( 0.0f,-1.0f),    
    glm::normalize(glm::vec2( 1.0f, 1.0f)),
    glm::normalize(glm::vec2(-1.0f, 1.0f)),
    glm::normalize(glm::vec2( 1.0f,-1.0f)),
    glm::normalize(glm::vec2(-1.0f,-1.0f))
};
int gradientMask2D = 7;
float sqr2 = 1.414213562373095;

float GetPerlinNoise2D(float x, float y, int frequency) {
    x *= frequency;
    y *= frequency;

    int ix0 = std::floor(x);
    int iy0 = std::floor(y);
    float tx0 = x - ix0;
    float ty0 = y - iy0;
    float tx1 = tx0 - 1.0f;
    float ty1 = ty0 - 1.0f;
    ix0 &= hashMask;
    iy0 &= hashMask;
    int ix1 = ix0 + 1;
    int iy1 = iy0 + 1;
    
    int h0 = hash[ix0];
    int h1 = hash[ix1];
    glm::vec2 g00 = gradients2D[hash[h0 + iy0] & gradientMask2D];
    glm::vec2 g10 = gradients2D[hash[h1 + iy0] & gradientMask2D];
    glm::vec2 g01 = gradients2D[hash[h0 + iy1] & gradientMask2D];
    glm::vec2 g11 = gradients2D[hash[h1 + iy1] & gradientMask2D];

    float v00 = glm::dot(g00, glm::vec2(tx0, ty0));
    float v10 = glm::dot(g10, glm::vec2(tx1, ty0));
    float v01 = glm::dot(g01, glm::vec2(tx0, ty1));
    float v11 = glm::dot(g11, glm::vec2(tx1, ty1));
    
    float tx = Smooth(tx0);
    float ty = Smooth(ty0);
    return Lerp(
        Lerp(v00, v10, tx),
        Lerp(v01, v11, tx),
        ty) * sqr2;
}
glm::vec3 gradients3D[16] = {
    glm::vec3( 1.0f, 1.0f, 0.0f),
    glm::vec3(-1.0f, 1.0f, 0.0f),
    glm::vec3( 1.0f,-1.0f, 0.0f),
    glm::vec3(-1.0f,-1.0f, 0.0f),
    glm::vec3( 1.0f, 0.0f, 1.0f),
    glm::vec3(-1.0f, 0.0f, 1.0f),
    glm::vec3( 1.0f, 0.0f,-1.0f),
    glm::vec3(-1.0f, 0.0f,-1.0f),
    glm::vec3( 0.0f, 1.0f, 1.0f),
    glm::vec3( 0.0f,-1.0f, 1.0f),
    glm::vec3( 0.0f, 1.0f,-1.0f),
    glm::vec3( 0.0f,-1.0f,-1.0f),
    
    glm::vec3( 1.0f, 1.0f, 0.0f),
    glm::vec3(-1.0f, 1.0f, 0.0f),
    glm::vec3( 0.0f,-1.0f, 1.0f),
    glm::vec3( 0.0f,-1.0f,-1.0f)
};

int gradientsMask3D = 15;


float GetPerlinNoise3D(float x, float y, float z, int frequency) {
    x *= frequency;
    y *= frequency;
    z *= frequency;

    int ix0 = (int)std::floor(x);
    int iy0 = (int)std::floor(y);
    int iz0 = (int)std::floor(z);
    float tx0 = x - ix0;
    float ty0 = y - iy0;
    float tz0 = z - iz0;
    float tx1 = tx0 - 1.0f;
    float ty1 = ty0 - 1.0f;
    float tz1 = tz0 - 1.0f;
    ix0 &= hashMask;
    iy0 &= hashMask;
    iz0 &= hashMask;
    int ix1 = ix0 + 1;
    int iy1 = iy0 + 1;
    int iz1 = iz0 + 1;
    
    int h0 = hash[ix0];
    int h1 = hash[ix1];
    int h00 = hash[h0 + iy0];
    int h10 = hash[h1 + iy0];
    int h01 = hash[h0 + iy1];
    int h11 = hash[h1 + iy1];
    glm::vec3 g000 = gradients3D[hash[h00 + iz0] & gradientsMask3D];
    glm::vec3 g100 = gradients3D[hash[h10 + iz0] & gradientsMask3D];
    glm::vec3 g010 = gradients3D[hash[h01 + iz0] & gradientsMask3D];
    glm::vec3 g110 = gradients3D[hash[h11 + iz0] & gradientsMask3D];
    glm::vec3 g001 = gradients3D[hash[h00 + iz1] & gradientsMask3D];
    glm::vec3 g101 = gradients3D[hash[h10 + iz1] & gradientsMask3D];
    glm::vec3 g011 = gradients3D[hash[h01 + iz1] & gradientsMask3D];
    glm::vec3 g111 = gradients3D[hash[h11 + iz1] & gradientsMask3D];

    float v000 = glm::dot(g000, glm::vec3(tx0, ty0, tz0));
    float v100 = glm::dot(g100, glm::vec3(tx1, ty0, tz0));
    float v010 = glm::dot(g010, glm::vec3(tx0, ty1, tz0));
    float v110 = glm::dot(g110, glm::vec3(tx1, ty1, tz0));
    float v001 = glm::dot(g001, glm::vec3(tx0, ty0, tz1));
    float v101 = glm::dot(g101, glm::vec3(tx1, ty0, tz1));
    float v011 = glm::dot(g011, glm::vec3(tx0, ty1, tz1));
    float v111 = glm::dot(g111, glm::vec3(tx1, ty1, tz1));

    float tx = Smooth(tx0);
    float ty = Smooth(ty0);
    float tz = Smooth(tz0);
    return Lerp(
        Lerp(Lerp(v000, v100, tx), Lerp(v010, v110, tx), ty),
        Lerp(Lerp(v001, v101, tx), Lerp(v011, v111, tx), ty),
        tz);
}

//Perlin Noise  
float GetFracNoise1D(float x, int frequency, int octaves, float lacunarity, float persistence ) {
    float sum = GetPerlinNoise1D(x, frequency);
    float amplitude = 1.0f;
    float range = 1.0f;

    for(int i=0; i<octaves; i++) {
        frequency *= lacunarity;
        amplitude *= persistence;
        range += amplitude;
        sum += GetPerlinNoise1D(x, frequency) * amplitude;
    }
    return sum / range;
}

float GetFracNoise2D(float x, float y,int frequency, int octaves, float lacunarity, float persistence) {
    float sum = GetPerlinNoise2D(x, y, frequency);
    float amplitude = 1.0f;
    float range = 1.0f;

    for(int i=0; i<octaves; i++) {
        frequency *= lacunarity;
        amplitude *= persistence;
        range += amplitude;
        sum += GetPerlinNoise2D(x , y, frequency) * amplitude;
    }
    return sum / range;
}

float GetFracNoise3D(float x, float y, float t,int frequency, int octaves, float lacunarity, float persistence ) {
    float sum = GetPerlinNoise3D(x, y, t,  frequency);
    float amplitude = 1.0f;
    float range = 1.0f;

    for(int i=0; i<octaves; i++) {
        frequency *= lacunarity;
        amplitude *= persistence;
        range += amplitude;
        sum += GetPerlinNoise3D(x, y, t,  frequency) * amplitude;
    }
    return sum / range;    
}
}
}