#include "TorranceSparrow.hpp"
#include "../Util/Geometry.hpp"

namespace KikooRenderer{
namespace OfflineRenderer {
TorranceSparrow::TorranceSparrow(glm::vec3 color) : BRDF(color) {

}

glm::vec3 TorranceSparrow::f(glm::vec3 wo, glm::vec3 wi) {
    refractionIndex = 1.5;

    float cosThetaO = AbsCosTheta(wo), cosThetaI = AbsCosTheta(wi);
    glm::vec3 wh = wi + wo;
    if (cosThetaI == 0 || cosThetaO == 0) return glm::vec3(0.);
    if (wh.x == 0 && wh.y == 0 && wh.z == 0) return glm::vec3(0.);

    wh = glm::normalize(wh);
    float F = this->FrDielectric(glm::dot(wi, wh), 1.0f,  refractionIndex);

    return R * microFacetsDistribution.D(wh) * microFacetsDistribution.G(wo, wi) * F /
           (4 * cosThetaI * cosThetaO);
}

}
}