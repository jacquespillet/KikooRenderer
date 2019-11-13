#pragma once
#include "Util/Common.h"

namespace KikooRenderer{
namespace OfflineRenderer {

inline float CosTheta(const glm::vec3 &w) { return w.z;}
inline float Cos2Theta(const glm::vec3 &w) { return w.z * w.z;}
inline float AbsCosTheta(const glm::vec3 &w) { return std::abs(w.z);}

inline float Sin2Theta(const glm::vec3 &w) { return std::max(0.0f, 1.0f - CosTheta(w));}
inline float SinTheta(const glm::vec3 &w) { return sqrt(Sin2Theta(w));}

inline float TanTheta(const glm::vec3 &w) { return SinTheta(w) / CosTheta(w);}
inline float Tan2Theta(const glm::vec3 &w) { return Sin2Theta(w) / Cos2Theta(w);}

inline float CosPhi(const glm::vec3 &w) {
    float sinTheta = SinTheta(w);
    return (sinTheta == 0) ? 1 : glm::clamp(w.x / sinTheta, -1.0f, 1.0f);
}

inline float SinPhi(const glm::vec3 &w) {
    float sinTheta = SinTheta(w);
    return (sinTheta == 0) ? 0 : glm::clamp(w.y / sinTheta, -1.0f, 1.0f);
}

inline float Cos2Phi(const glm::vec3 &w) {
    return CosPhi(w) * CosPhi(w);
}
inline float Sin2Phi(const glm::vec3 &w) {
    return SinPhi(w) * SinPhi(w);
}

inline float CosDPhi(const glm::vec3 &wa, const glm::vec3 &wb) {
    return glm::clamp((wa.x * wb.x + wa.y * wb.y) /
                 std::sqrt((wa.x * wa.x + wa.y * wa.y) *
                           (wb.x * wb.x + wb.y * wb.y)), -1.0f, 1.0f);
}

}
}