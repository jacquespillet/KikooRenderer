#pragma once

#include "Util/Common.h"

namespace KikooRenderer {
namespace Util {

void Random(std::vector<glm::vec2>& samples, int numSamples);
void Jitter(std::vector<glm::vec2>& samples, int numSamples);
void NRooks(std::vector<glm::vec2>& samples, int numSamples);
void MultiJitter(std::vector<glm::vec2>& samples, int numSamples);
void Shuffle(std::vector<glm::vec2>& samples, int numSamples);

void BoxFilter(std::vector<glm::vec2>& samples, int numSamples);
void TentFilter(std::vector<glm::vec2>& samples, int numSamples);
void CubicSplineFilter(std::vector<glm::vec2>& samples, int numSamples);

void Random(std::vector<float>& samples, int numSamples);
void Jitter(std::vector<float>& samples, int numSamples);
void Shuffle(std::vector<float>& samples, int numSamples);

float Solve(float r);

float cubicFilter(float x);

}
}