#include "Material.hpp"
#include "../Shapes/Shape.hpp"
#include "Geometry/Util.h"
#include <stb_image.h>

#include "../Util/Geometry.hpp"


namespace KikooRenderer{
namespace OfflineRenderer {

    Material::Material(glm::vec4 albedo) : albedo(albedo), brdf(albedo) {}
    bool Material::Scatter(KikooRenderer::Geometry::Ray in,  Point point, glm::vec3& attenuation, KikooRenderer::Geometry::Ray& scattered) {
        // glm::vec3 target = point.position + point.normal;
        glm::vec3 target = point.position + point.normal + Geometry::randomInSphere();
        scattered = Geometry::Ray(point.position, target-point.position);
        if(hasTexture) {
            float texCoordX = std::min((float)width, std::max(0.0f, point.uv.x * (float)width));
            float texCoordY = std::min((float)width, std::max(0.0f, point.uv.y * (float)height));

            attenuation.r = (float)(textureData[std::floor(texCoordY * width + texCoordX)]) / 255;
            attenuation.g = (float)(textureData[std::floor(texCoordY * width + texCoordX + 1)]) / 255;
            attenuation.b = (float)(textureData[std::floor(texCoordY * width + texCoordX + 2)]) / 255;
            // attenuation.r = point.uv.x;
            // attenuation.g = point.uv.y;
            // attenuation.b = 0;
        } else {
            //DEBUG
            // attenuation = target;
            // attenuation = point.normal;
            // attenuation = point.position;
            // std::cout << glm::to_string(attenuation) << std::endl;
            // attenuation = glm::vec3 (point.position.b, 0, 0 );

            if(useBrdf) {
                glm::mat4 worldToTangentMatrix(1);
                worldToTangentMatrix[0] = glm::vec4(glm::normalize(point.tangent), 1);
                worldToTangentMatrix[1] = glm::vec4(glm::normalize(point.bitangent), 1);
                worldToTangentMatrix[2] = glm::vec4(glm::normalize(point.normal), 1);
                worldToTangentMatrix[3] = glm::vec4(0, 0, 0, 1);
                
                // glm::vec4 cameraToPoint = glm::vec4(point.position - in.origin, 0);
                glm::vec4 cameraToPoint = glm::vec4(in.origin - point.position, 0);
                cameraToPoint = worldToTangentMatrix * cameraToPoint;
                
                float numSamples = 1;
                attenuation = brdf.Sample_f(cameraToPoint, &target, glm::vec2(1), &numSamples);
                // std::cout << glm::to_string(albedo) << "  " << glm::to_string(attenuation) << std::endl;
                // attenuation = albedo;
            } else {
                attenuation = albedo;
            }
        }
        
        if(useBrdf)return glm::dot(scattered.direction, point.normal) > 0;
        else return true;
    }

    glm::vec3 Material::emitted() {
        if(emitter) return glm::vec3(5);
        else return glm::vec3(0);
    }

    void Material::LoadTexture(std::string fileName) {
        stbi_set_flip_vertically_on_load(true);  
		unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
		if(data) {
			std::cout << "Texture:Constructor:SUCCESS" << std::endl;
            textureData = std::vector<uint8_t>(data, data + (width * height * nrChannels));
            hasTexture = true;
        }else
		{
			std::cout << "Texture:Constructor: ERROR::Failed to load texture" << std::endl;
			return;
		} 
		stbi_image_free(data);
    }

}
}