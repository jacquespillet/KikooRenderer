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
        
        virtual glm::vec3 Sample_f(const glm::vec3 &wo, glm::vec3 *wi, const glm::vec2 &sample, float *pdf);
        virtual glm::vec3 Sample_f_Transmission(const glm::vec3 &wo, glm::vec3 *wi, const glm::vec2 &sample, float *pdf);
        virtual glm::vec3 OrenNayar(const glm::vec3 &wo, glm::vec3& wi);

        virtual glm::vec3 Generate(glm::vec3 in, float* pdf);
        virtual float Evaluate(glm::vec3 wi, glm::vec3 wo);
        virtual float PDF(glm::vec3 wo, glm::vec3 wh);

    protected:
        int type;

        float refractionIndex = 1.05;
        float k = 0.1; //Absorbtion coefficient of conductor materials
        glm::vec3 R;
        
        float FrDielectric(float cosThetaI, float etaI, float etaT);
        float FrConductor(float cosThetaI, float etaI, float etaT);
};
}
}