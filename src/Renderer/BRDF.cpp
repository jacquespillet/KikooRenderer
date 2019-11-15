#include "BRDF.hpp"
#include "Util/Geometry.hpp"
namespace KikooRenderer{
namespace OfflineRenderer {

BRDF::BRDF(glm::vec3 color) : R(color){}

//Returns the result of the fresnel equations. 
//Computes an average of the parallel & perpendicular fresnel components (we assume that the lights are unpolarized)
//Gives the frac part of the reflected light from the incident angle Theta
//EtaI is the refraction index of the outter medium
//EtaT is the refraction index of the current medium
//To get the amount of transmitted light, we can just do 1 - F
float BRDF::FrDielectric(float cosThetaI, float etaI, float etaT) {
    cosThetaI = glm::clamp(cosThetaI, -1.0f, 1.0f);

    float sinThetaI = std::sqrt(std::max(0.0f, 1.0f - cosThetaI * cosThetaI));
    float sinThetaT = etaI / etaT * sinThetaI;
    
    if(sinThetaT >= 1) return 1;

    float cosThetaT = std::sqrt(std::max(0.0f, 1.0f-sinThetaT * sinThetaT));


    bool entering = cosThetaI > 0.f;
    if (!entering) {
        std::swap(etaI, etaT);
        cosThetaI = std::abs(cosThetaI);
    }

    float rParallel      = ((etaT * cosThetaI) - (etaI * cosThetaT)) /
                           ((etaT * cosThetaI) + (etaI * cosThetaT));
    
    float rPerpendicular = ((etaI * cosThetaI) - (etaT * cosThetaT)) /
                           ((etaI * cosThetaI) + (etaT * cosThetaT));

    return (rParallel * rParallel + rPerpendicular * rPerpendicular) * 0.5;
}

float BRDF::FrConductor(float cosThetaI, float etaI, float etaT) {
    cosThetaI = glm::clamp(cosThetaI, -1.0f, 1.0f);
    float eta = etaT / etaI;
    float etak = k / etaI;

    float cosThetaI2 = cosThetaI * cosThetaI;
    float sinThetaI2 = 1. - cosThetaI2;
    float eta2 = eta * eta;
    float etak2 = etak * etak;

    float t0 = eta2 - etak2 - sinThetaI2;
    float a2plusb2 = std::sqrt(t0 * t0 + 4 * eta2 * etak2);
    float t1 = a2plusb2 + cosThetaI2;
    float a = std::sqrt(0.5f * (a2plusb2 + t0));
    float t2 = (float)2 * cosThetaI * a;
    float Rs = (t1 - t2) / (t1 + t2);

    float t3 = cosThetaI2 * a2plusb2 + sinThetaI2 * sinThetaI2;
    float t4 = t2 * sinThetaI2;
    float Rp = Rs * (t3 - t4) / (t3 + t4);

    return 0.5 * (Rp + Rs);
}

//Computed the BRDF
//Returns the specular reflection of the surface given the view direction
//Assumes the light is always located at the reflection of the view angle
//It is called Perfect specularity
glm::vec3 BRDF::Sample_f(const glm::vec3 &wo, glm::vec3 *wi, const glm::vec2 &sample, float *pdf) {
    *wi = glm::vec3(-wo.x, -wo.y, wo.z);
    *pdf = 1;

    glm::vec3 res = this->FrDielectric( CosTheta(*wi),  1.0f,  refractionIndex)  * R / AbsCosTheta(*wi);
    // glm::vec3 res = this->FrConductor( CosTheta(*wi),  1.0f,  refractionIndex)  * R / AbsCosTheta(*wi);
    return res;
}

}
}