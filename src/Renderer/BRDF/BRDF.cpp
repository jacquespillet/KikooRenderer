#include "BRDF.hpp"
#include "../Util/Geometry.hpp"
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

    refractionIndex = 1.5;
    k = 0.5;
    // glm::vec3 res = this->FrDielectric( CosTheta(*wi),  1.0f,  refractionIndex)  * R / AbsCosTheta(*wi);
    glm::vec3 res = this->FrConductor( CosTheta(*wi),  1.0f,  refractionIndex)  * R / AbsCosTheta(*wi);
    return res; 
}

glm::vec3 BRDF::Sample_f_Transmission(const glm::vec3 &wo, glm::vec3 *wi, const glm::vec2 &sample, float *pdf) {
    refractionIndex = 1.5;
    
    bool entering = CosTheta(wo) > 0;
    float etaI = entering ? 1 : refractionIndex;
    float etaT = entering ? refractionIndex : 1;

    *wi = glm::refract(-wo, glm::faceforward(glm::vec3(0, 0, 1), -wo, glm::vec3(0, 0, 1)), etaI / etaT);

    *pdf = 1;
    glm::vec3 ft = R * (glm::vec3(1.) - FrDielectric(CosTheta(*wi), etaI, etaT));
    // ft *= (etaI * etaI) / (etaT * etaT);

    return ft / AbsCosTheta(*wi); 
}

glm::vec3 BRDF::OrenNayar(const glm::vec3 &wo, glm::vec3& wi) {
    float sigma = 50;
    float sigma2 = sigma * sigma;
    float A = 1.f - (sigma2 / (2.f * (sigma2 + 0.33f)));
    float B = 0.45f * sigma2 / (sigma2 + 0.09f);

    float sinThetaI = SinTheta(wi);
    float sinThetaO = SinTheta(wo);    

    float maxCos = 0;
    if (sinThetaI > 1e-4 && sinThetaO > 1e-4) {
        float sinPhiI = SinPhi(wi), cosPhiI = CosPhi(wi);
        float sinPhiO = SinPhi(wo), cosPhiO = CosPhi(wo);
        float dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
        maxCos = std::max((float)0, dCos);
    }    

    float sinAlpha, tanBeta;
    if (AbsCosTheta(wi) > AbsCosTheta(wo)) {
        sinAlpha = sinThetaO;
        tanBeta = sinThetaI / AbsCosTheta(wi);
    } else {
        sinAlpha = sinThetaI;
        tanBeta = sinThetaO / AbsCosTheta(wo);
    }

    return R * (1.0 / PI) * (A + B * maxCos * sinAlpha * tanBeta);

}

//-------------------------------------------------------
float DistributionGGX(glm::vec3 N, glm::vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(glm::vec3 N, glm::vec3 V, glm::vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

glm::vec3 fresnelSchlick(float cosTheta, glm::vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}  


float BRDF::Evaluate(glm::glm::vec3 wi, glm::glm::vec3 wo) {
    float roughness = 0.5;

    glm::vec3 N(0, 0, 1);
    glm::vec3 V = wi;
    glm::vec3 L = wo;

    glm::vec3 F0 = glm::vec3(0.04); 
    
    // // reflectance equation
    glm::vec3 Lo = glm::vec3(0.0);
    // calculate per-light radiance
    glm::vec3 H = normalize(V + L);        
    
    // // cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(N, V, L, roughness);      
    glm::vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
    
    glm::vec3 kS = F;
    glm::vec3 kD = glm::vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  
    
    glm::vec3 DFG    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    glm::vec3 specular     = DFG / max(denominator, 0.001);  
        
    return specular.x;
}

glm::vec3 BRDF::Generate(glm::vec3 in) {
    //https://agraphicsguy.wordpress.com/2015/11/01/sampling-microfacet-brdf/

    float alpha = 0.5;
    double random = ((double) rand()) / (double) RAND_MAX;
    double numerator = 1 - random;
    double denominator = random * (alpha * alpha - 1) + 1
    double theta = acos(sqrt((numerator / denominator)));

    //??
    double phi = -atan(in.x / in.z);

    glm::vec3 out;
    out.x = sin(theta)*cos(phi);
    out.y = sin(theta)*sin(phi);
    out.z = cos(theta);
    out = glm::normalize(out);

    return out;
}

float BRDF::PDF(glm::vec3 wo) {
    //https://agraphicsguy.wordpress.com/2015/11/01/sampling-microfacet-brdf/
    
    double cosTheta = glm::dot(wo, glm::vec3(1, 0, 0));
    double theta = acos(cosTheta);

    double alpha = 0.5;

    double numerator = 2 * alpha * alpha * cosTheta * sin(theta);
    double denominator = std::pow(((alpa * alpha - 1) * cosTheta * cosTheta + 1), 2);

    return return numerator / denominator;
}

}
}