#include "MicroFacets.hpp"
#include "Util/Geometry.hpp"

namespace KikooRenderer{
namespace OfflineRenderer {

MicroFacets::MicroFacets(){
    roughness = 1;
    SetRoughness(roughness);
}

void MicroFacets::SetRoughness(float r) {
    this->roughness = r;
        roughness = std::max(roughness, (float)1e-3);
        float x = std::log(roughness);
        float a =  1.62142f + 0.819955f * x + 0.1734f * x * x +
               0.0171201f * x * x * x + 0.000640711f * x * x * x * x;    
        alphax = a;
        alphay = a;
}

float MicroFacets::BeckmannDistribution(glm::vec3 wh) {
    float tan2Theta = Tan2Theta(wh);

    if(std::isinf(tan2Theta)) return 0;

    float cos4Theta = Cos2Theta(wh) * Cos2Theta(wh);

    return std::exp(-tan2Theta * (Cos2Phi(wh) / (alphax * alphax) +
                                  Sin2Phi(wh) / (alphay * alphay))) /
        (PI * alphax * alphay * cos4Theta);
}

float MicroFacets::TrowbridgeReitzDistribution(glm::vec3 wh) {
    float tan2Theta = Tan2Theta(wh);
    if (std::isinf(tan2Theta)) return 0.;
    const float cos4Theta = Cos2Theta(wh) * Cos2Theta(wh);
    float e = (Cos2Phi(wh) / (alphax * alphax) +
               Sin2Phi(wh) / (alphay * alphay)) * tan2Theta;
    return 1 / (PI * alphax * alphay * cos4Theta * (1 + e) * (1 + e));
}

float MicroFacets::D(glm::vec3 wh) {
    return TrowbridgeReitzDistribution(wh);
}


float MicroFacets::Lambda(glm::vec3 w) {
    float absTanTheta = std::abs(TanTheta(w));
    if (std::isinf(absTanTheta)) return 0.;
    
    float alpha = std::sqrt(Cos2Phi(w) * alphax * alphax +
                        Sin2Phi(w) * alphay * alphay);
    

    float a = 1 / (alpha * absTanTheta);
    if (a >= 1.6f)
        return 0;
    return (1 - 1.259f * a + 0.396f * a * a) /
           (3.535f * a + 2.181f * a * a);    
}

float MicroFacets::G(glm::vec3 wo, glm::vec3 wi) {
    return 1 / (1 + Lambda(wo) + Lambda(wi));
}

float MicroFacets::G1(glm::vec3 wh) {
    return 1 / (1 + Lambda(wh));
}

}
}