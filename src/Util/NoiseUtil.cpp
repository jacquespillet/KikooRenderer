#include "NoiseUtil.hpp"


namespace KikooRenderer {
namespace Util {

float Lerp(float x, float y, float t) {
    return x * (1 - t) + y * t;
}
float Smooth (float t) {
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float SmoothDerivative (float t) {
    return 30.0f * t * t * (t * (t - 2.0f) + 1.0f);
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

NoiseSample GetValueNoise1D(float x, float frequency) {
    x *= frequency;

    int i0 = (int)std::floor(x);

    float t = x - i0;

    i0 &= hashMask;
    int i1 = i0 + 1;

    int h0 = hash[i0];
    int h1 = hash[i1];

    float dt = SmoothDerivative(t);    
    t = Smooth(t);

    float a = h0;
    float b = h1 - h0;

    NoiseSample sample;
    sample.value = a + b * t / (float)hashMask;
    sample.derivative = glm::vec3(b * dt, 0, 0);
    sample.derivative *= frequency;
    sample.derivative /= (float)hashMask;
    return sample; 
}


NoiseSample GetValueNoise2D(float x, float y, float frequency) {
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
    float dtx = SmoothDerivative(tx);
    float dty = SmoothDerivative(ty);    
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

    float a = h00;
    float b = h10 - h00;
    float c = h01 - h00;
    float d = h11 - h01 - h10 + h00;    

    NoiseSample sample;
    sample.value = (a + b * tx + (c + d * tx) * ty) / (float)hashMask;
    sample.derivative = glm::vec3((b + d * ty) * dtx, (c + d * tx) * dty, 0);
    sample.derivative *= frequency;
    sample.derivative /= (float)hashMask;
    return sample; 
}

NoiseSample GetValueNoise3D(float x, float y, float t, float frequency) {
    x *= frequency;
    y *= frequency;
    t *= frequency;

    int ix0 = (int)std::floor(x);
    int iy0 = (int)std::floor(y);
    int it0 = (int)std::floor(t);

    float tx = x - ix0;
    float ty = y - iy0;
    float tz = t - it0;    
    float dtx = SmoothDerivative(tx);
    float dty = SmoothDerivative(ty);
    float dtz = SmoothDerivative(tz);    
    tx = Smooth(tx);
    ty = Smooth(ty);
    tz = Smooth(tz);  

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

    float a = h000;
    float b = h100 - h000;
    float c = h010 - h000;
    float d = h001 - h000;
    float e = h110 - h010 - h100 + h000;
    float f = h101 - h001 - h100 + h000;
    float g = h011 - h001 - h010 + h000;
    float h = h111 - h011 - h101 + h001 - h110 + h010 + h100 - h000;    
    
    NoiseSample sample;
    sample.value = (a + b * tx + (c + e * tx) * ty + (d + f * tx + (g + h * tx) * ty) * tz) * (1.0f / hashMask);
    sample.derivative = glm::vec3((b + e * ty + (f + h * ty) * tz) * dtx, 
                                  (c + e * tx + (g + h * tx) * tz) * dty,
                                  (d + f * tx + (g + h * tx) * ty) * dtz);
    sample.derivative *= frequency;
    sample.derivative /= (float)hashMask;
    return sample; 
}


float gradients1D[2] = {
		-1, 1
};
int gradientMask1D = 1;

NoiseSample GetPerlinNoise1D(float x, float frequency) {
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

    float dt = SmoothDerivative(t0);
    float t = Smooth(t0);
    
    float a = v0;
    float b = v1 - v0;

    float da = g0;
    float db = g1 - g0;

    NoiseSample sample;
    sample.value = (a + b * t) * 2;
    sample.derivative = glm::vec3(da + db * t + b * dt, 0, 0);
    sample.derivative *= frequency * 2;
    return sample; 

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

NoiseSample GetPerlinNoise2D(float x, float y, float frequency) {
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
    float dtx = SmoothDerivative(tx0);
    float dty = SmoothDerivative(ty0);

    float a = v00;
    float b = v10 - v00;
    float c = v01 - v00;
    float d = v11 - v01 - v10 + v00;

    glm::vec2 da = g00;
    glm::vec2 db = g10 - g00;
    glm::vec2 dc = g01 - g00;
    glm::vec2 dd = g11 - g01 - g10 + g00;

    NoiseSample sample;
    sample.value = (a + b * tx + (c + d * tx) * ty) * sqr2;

    glm::vec2 deriv = da + db * tx + (dc + dd * tx) * ty;
    sample.derivative = glm::vec3(deriv.x, deriv.y, 0);
    sample.derivative.x += (b + d * ty) * dtx;
    sample.derivative.y += (c + d * tx) * dty;  
    sample.derivative *= frequency * sqr2;
    return sample; 

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


NoiseSample GetPerlinNoise3D(float x, float y, float z, float frequency) {
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
    float dtx = SmoothDerivative(tx0);
    float dty = SmoothDerivative(ty0);
    float dtz = SmoothDerivative(tz0);


    float a = v000;
    float b = v100 - v000;
    float c = v010 - v000;
    float d = v001 - v000;
    float e = v110 - v010 - v100 + v000;
    float f = v101 - v001 - v100 + v000;
    float g = v011 - v001 - v010 + v000;
    float h = v111 - v011 - v101 + v001 - v110 + v010 + v100 - v000;

    glm::vec3 da = g000;
    glm::vec3 db = g100 - g000;
    glm::vec3 dc = g010 - g000;
    glm::vec3 dd = g001 - g000;
    glm::vec3 de = g110 - g010 - g100 + g000;
    glm::vec3 df = g101 - g001 - g100 + g000;
    glm::vec3 dg = g011 - g001 - g010 + g000;
    glm::vec3 dh = g111 - g011 - g101 + g001 - g110 + g010 + g100 - g000;    

    NoiseSample sample;
    sample.value = a + b * tx + (c + e * tx) * ty + (d + f * tx + (g + h * tx) * ty) * tz;
    sample.derivative = da + db * tx + (dc + de * tx) * ty + (dd + df * tx + (dg + dh * tx) * ty) * tz;
    sample.derivative.x += (b + e * ty + (f + h * ty) * tz) * dtx;
    sample.derivative.y += (c + e * tx + (g + h * tx) * tz) * dty;
    sample.derivative.z += (d + f * tx + (g + h * tx) * ty) * dtz;
    sample.derivative *= frequency;
    return sample; 
}

//Perlin Noise  
NoiseSample GetFracNoise1D(float x, float frequency, int octaves, float lacunarity, float persistence ) {
    NoiseSample sum = GetPerlinNoise1D(x, frequency);
    float amplitude = 1.0f;
    float range = 1.0f;

    for(int i=0; i<octaves; i++) {
        frequency *= lacunarity;
        amplitude *= persistence;
        range += amplitude;
        NoiseSample sample = GetPerlinNoise1D(x, frequency);
        sum.value += sample.value * amplitude;
        sum.derivative += sample.derivative * amplitude;
    }
    sum.value /= range;
    sum.derivative /= range;
    return sum;
}

NoiseSample GetFracNoise2D(float x, float y, float frequency, int octaves, float lacunarity, float persistence) {
    NoiseSample sum  = GetPerlinNoise2D(x, y, frequency);
    float amplitude = 1.0f;
    float range = 1.0f;

    for(int i=0; i<octaves; i++) {
        frequency *= lacunarity;
        amplitude *= persistence;
        range += amplitude;
        NoiseSample sample = GetPerlinNoise2D(x , y, frequency);
        sum.value += sample.value * amplitude;
        sum.derivative += sample.derivative * amplitude;
    }
    sum.value /= range;
    sum.derivative /= range;
    return sum;
}

NoiseSample GetFracNoise3D(float x, float y, float t,float frequency, int octaves, float lacunarity, float persistence ) {
    NoiseSample sum = GetPerlinNoise3D(x, y, t,  frequency);
    float amplitude = 1.0f;
    float range = 1.0f;

    for(int i=0; i<octaves; i++) {
        frequency *= lacunarity;
        amplitude *= persistence;
        range += amplitude;
        NoiseSample sample = GetPerlinNoise3D(x, y, t,  frequency);
        sum.value += sample.value * amplitude;
        sum.derivative += sample.derivative * amplitude;
    }
    sum.value /= range;
    sum.derivative /= range;
    return sum;    
}

/////////////////___________________________________
// 2D voronoi noise.
float r(float n)
{
 	return glm::fract(cos(n*89.42)*343.42);
}

glm::vec3 r(glm::vec3 n)
{
 	return glm::vec3(r(n.x*23.62-300.0+n.y*34.35 + n.z*56.35),r(n.x*45.13+256.0+n.y*38.89 + n.z*32.35), r(n.x*45.13+256.0+n.y*38.89 + n.z*43.35)); 
}

float voronoi2D(glm::vec3 n, float frequency)
{
    n *= frequency;
    float dis = 2.0;
    
    for (int z= -1; z <= 1; z++) 
    {
        for (int y= -1; y <= 1; y++) 
        {
            for (int x= -1; x <= 1; x++) 
            {
                glm::vec3 p = glm::floor(n) + glm::vec3(x,y, z); //Neighbour center
                glm::vec3 neighbour = r(glm::mod(p, frequency)) + glm::vec3(x, y, z); //Neighbour point
                float d = glm::length(neighbour - glm::fract(n));
                
                if (dis > d) {
                    dis = d;   
                }
            }
        }
    }
    
    return 1.0 - dis;
}
/////////////////___________________________________

float GetWorleyNoise3D(float x, float y, float z, float frequency) {
    float value = voronoi2D(glm::vec3(x, y, z), frequency);
    return value;
}





//----------------------------------------------------------------------------------------
float Hash(glm::vec3 p, float scale)
{
	// This is tiling part, adjusts with the scale...
	p = glm::mod(p, scale);
	return glm::fract(glm::sin(glm::dot(p, glm::vec3(27.16898, 38.90563, 30.23908))) * 5151.5473453);
}

//----------------------------------------------------------------------------------------
float Noise(glm::vec3 p, float scale )
{
	glm::vec3 f;
	
	p *= scale;

	
	f = fract(p);		// Separate integer from fractional
    p = floor(p);
	
    f = f*f*(3.0f-2.0f*f);	// Cosine interpolation approximation
	
    float res = glm::mix( glm::mix( glm::mix(
                          Hash(p, 				         scale), 
                          Hash(p + glm::vec3(1.0, 0.0,0.0), scale), f.x),
                     glm::mix( Hash(p + glm::vec3(0.0, 1.0,0.0), scale), 
                          Hash(p + glm::vec3(1.0, 1.0,0.0), scale), f.x), f.y),
                glm::mix( glm::mix( Hash(p + glm::vec3(0.0, 0.0,1.0), scale), 
                          Hash(p + glm::vec3(1.0, 0.0,1.0), scale), f.x),
                     glm::mix( Hash(p + glm::vec3(0.0, 1.0,1.0), scale), 
                          Hash(p + glm::vec3(1.0, 1.0,1.0), scale), f.x), f.y), f.z );                        
    return res;
}

//----------------------------------------------------------------------------------------
float fBm(glm::vec3 p)
{
    p += glm::vec3(sin(0.7), cos(0.45), sin(0.21))*(.1);
	float f = 0.0;
	// Change starting scale to any integer value...
	float scale = 10.;
    p = mod(p, scale);
	float amp   = 0.1;
	
	for (int i = 0; i < 5; i++)
	{
		f += Noise(p, scale) * amp;
		amp *= .5;
		// Scale must be multiplied by an integer value...
		scale *= 2.;
	}
	return glm::min(f, 1.0f);
}

float GetPerlinWorleyNoise(float x, float y, float z, float frequency) {
    float w = (fBm(glm::vec3(x, y, z)))
            * ((voronoi2D(glm::vec3(x, y, z), 1)) 
            + (0.5 * voronoi2D(glm::vec3(x, y, z), 2.)) 
            + (0.25 * voronoi2D(glm::vec3(x, y, z), 4.)));

    return (w / 4);

    // float w =  ((voronoi2D(glm::vec3(x, y, z), 1)) 
    //         + (0.5 * voronoi2D(glm::vec3(x, y, z), 2.)) 
    //         + (0.25 * voronoi2D(glm::vec3(x, y, z), 4.)));

    // return (w/3);
}

}
}