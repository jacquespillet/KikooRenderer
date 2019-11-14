#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"

namespace KikooRenderer{
namespace OfflineRenderer {

class BRDF {
    public: 
        BRDF(glm::vec3 color);
        glm::vec3 f(glm::vec3 wo, glm::vec3 wi);
        glm::vec3 rho(const glm::vec3 wo, int numSamples, glm::vec2* samples);
        // glm::vec3 rho(const glm::vec3 wo, int numSamples, glm::vec2* samples);
        glm::vec3 Sample_f(const glm::vec3 &wo, glm::vec3 *wi, const glm::vec2 &sample, float *pdf);
        glm::vec3 Sample_f_Transmission(const glm::vec3 &wo, glm::vec3 *wi, const glm::vec2 &sample, float *pdf);

    private:
        int type;

        float refractionIndex = 1.1;
        float k = 10; //Absorbtion coefficient of conductor materials
        glm::vec3 R;
        
        float FrDielectric(float cosThetaI, float etaI, float etaT);
        float FrConductor(float cosThetaI, float etaI, float etaT);
};

}
}