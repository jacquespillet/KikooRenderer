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

//Computed the BRDF
//Returns the specular reflection of the surface given the view direction
//Assumes the light is always located at the reflection of the view angle
//It is called Perfect specularity
glm::vec3 BRDF::Sample_f(const glm::vec3 &wo, glm::vec3 *wi, const glm::vec2 &sample, float *pdf) {
    if(wi == nullptr) *wi = glm::vec3(-wo.x, -wo.y, wo.z);
    *pdf = 1;

    glm::vec3 res = this->FrDielectric( CosTheta(*wi),  1.0f,  refractionIndex)  * R / AbsCosTheta(*wi);
    return res;
}        

}
}