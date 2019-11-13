#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"

namespace KikooRenderer{
namespace OfflineRenderer {

class BRDF {
    public: 
        BRDF();
        glm::vec3 f(glm::vec3 wo, glm::vec3 wi);
        glm::vec3 rho(const glm::vec3 wo, int numSamples, glm::vec2* samples);
        // glm::vec3 rho(const glm::vec3 wo, int numSamples, glm::vec2* samples);
    private:
        int type;

        float FrDielectric(float cosThetaI, float etaI, float etaT);
};

}
}