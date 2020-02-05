#include "Sample.hpp"
#include "RandomUtil.hpp"

namespace KikooRenderer {
namespace Util {

float Solve(float r) {
    float u = r;
    for(int i=0; i<5; i++) {
        u = (11.0f * r + u * u * (6.0f + u * (8.0f - 9.0f * u))) / 
            (4.0f + 12.0f * u * (1.0f + u * (1.0f - u)));
    }

    return u;
}

float cubicFilter(float x) {
    if (x < 1.0f / 24.0f) {
        return std::pow(24 * x, 0.25f) - 2.0f;
    } else if (x < 0.5) {
        return Solve(24.0f * (x - 1.0f / 24.0f) / 11.0f) - 1.0f;
    } else if (x < 23.0f / 24.0f) {
        return 1.0f - Solve(24.0f * (23.0f / 24.0f - x) / 11.0f);
    } else {
        return 2 - std::pow(24.0f * (1.0f - x), 0.25f);
    }
}


void Random(std::vector<glm::vec2>& samples, int numSamples) {
    if(samples.size() != numSamples) samples.resize(numSamples);

    for(int i=0; i<numSamples; i++) {
        samples[i].x = GetRand();
        samples[i].y = GetRand();
    }
}


void Jitter(std::vector<glm::vec2>& samples, int numSamples) {
    if(samples.size() != numSamples) samples.resize(numSamples);

    int sqrtSamples = (int)(std::sqrt(numSamples));

    for(int i=0; i<sqrtSamples; i++) {
        for(int j=0; j<sqrtSamples; j++) {
            float x = ((float)i + GetRand()) / (float)sqrtSamples;
            float y = ((float)j + GetRand()) / (float)sqrtSamples;

            samples[i * sqrtSamples + j].x = x;
            samples[i * sqrtSamples + j].y = y;
        }        
    }
}

void NRooks(std::vector<glm::vec2>& samples, int numSamples) {
    if(samples.size() != numSamples) samples.resize(numSamples);

    //Put samples on diagonal
    for(int i=0; i<numSamples; i++) {
        samples[i].x = ((float)i + GetRand()) / (float)(numSamples);
        samples[i].y = ((float)i + GetRand()) / (float)(numSamples);
    }

    //Set the x of the current with the x of another existing one, and inversely
    for(int i=numSamples-2; i>=0; i--) {
        int targetX = int(GetRand() * (float)i);
        float tempX = samples[i+1].x;

        samples[i+1].x = samples[targetX].x;
        samples[targetX].x = tempX;
    }
}

void MultiJitter(std::vector<glm::vec2>& samples, int numSamples) {
    if(samples.size() != numSamples) samples.resize(numSamples);

    int sqrtSamples = (int)std::sqrt(numSamples);
    float subCellWidth = 1.0f / (float)(numSamples);

    for(int i=0; i<sqrtSamples;i++) {
        for(int j=0; j<sqrtSamples;j++) {
            samples[i * sqrtSamples + j].x = i * sqrtSamples * subCellWidth + j * subCellWidth + GetRand() * subCellWidth;
            samples[i * sqrtSamples + j].y = j * sqrtSamples * subCellWidth + i * subCellWidth + GetRand() * subCellWidth;
        }
    }

    for(int i=0; i<sqrtSamples;i++) {
        for(int j=0; j<sqrtSamples;j++) {
            int k = j + int(GetRand() * (sqrtSamples - j - 1));
            float t = samples[i * sqrtSamples + j].x;
            samples[i * sqrtSamples + j].x = samples[i * sqrtSamples + k].x;
            samples[i * sqrtSamples + k].x = t;

            
            k = j + int(GetRand() * (sqrtSamples - j - 1));
            t = samples[j * sqrtSamples + i].y;
            samples[j * sqrtSamples + i].y = samples[k * sqrtSamples + i].y;
            samples[k * sqrtSamples + i].y = t;
        }
    }
}

void Shuffle(std::vector<glm::vec2>& samples, int numSamples) {
    for(int i=numSamples-2; i>=0; i--) {
        int target  = (int)(GetRand() * (float)i);
        glm::vec2 tmp = samples[i+1];
        samples[i+1] = samples[target];
        samples[target] = tmp;
    }
}


//Resample from 0->1 to -0.5 -> 0.5
void BoxFilter(std::vector<glm::vec2>& samples, int numSamples) {
    for(int i=0; i<numSamples; i++) {
        samples[i].x = samples[i].x-0.5f;
        samples[i].y = samples[i].y-0.5f;
    }
}

//Resample from 0->1 to -0.5 -> 0.5 using tent filter
void TentFilter(std::vector<glm::vec2>& samples, int numSamples) {
    for(int i=0; i<numSamples; i++) {
        float x = samples[i].x;
        float y = samples[i].y;

        if(x < -0.5) samples[i].x = (float)std::sqrt(2.0 * (float)x) - 1.0f;
        else samples[i].x = 1.0 - (float)std::sqrt(2.0 - 2.0 * (float)x);

        if(y < -0.5) samples[i].y = (float)std::sqrt(2.0 * (float)y) - 1.0f;
        else samples[i].y = 1.0 - (float)std::sqrt(2.0 - 2.0 * (float)y);
    }
}

//Resample from 0->1 to -0.5 -> 0.5 using cubic spline filter
void CubicSplineFilter(std::vector<glm::vec2>& samples, int numSamples) {
    for(int i=0; i<numSamples; i++) {
        float x = samples[i].x;
        float y = samples[i].y;

        samples[i].x = cubicFilter(x);
        samples[i].y = cubicFilter(y);
    }
}

void Random(std::vector<float>& samples, int numSamples) {
    if(samples.size() != numSamples) samples.resize(numSamples);

    for(int i=0; i<numSamples; i++) {
        samples[i] = GetRand();
    }
}

void Jitter(std::vector<float>& samples, int numSamples) {
    if(samples.size() != numSamples) samples.resize(numSamples);

    for(int i=0; i<numSamples; i++) {
        samples[i] = ((float)i + GetRand()) / (float)numSamples;
    }
}

void Shuffle(std::vector<float>& samples, int numSamples) {
    for(int i=numSamples-2; i>=0; i--) {
        int target = (int)(GetRand() * (float)i);
        float temp =samples[i+1];

        samples[i+1] = samples[target];
        samples[target] = temp;
    }
}



}
}