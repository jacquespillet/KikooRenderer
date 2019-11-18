#include "Material.hpp"
#include "../Shapes/Shape.hpp"
#include "Geometry/Util.h"
#include <stb_image.h>

#include "../Util/Geometry.hpp"


namespace KikooRenderer{
namespace OfflineRenderer {

    Material::Material(glm::vec4 albedo) : albedo(albedo), brdf(albedo) {}
    bool Material::Scatter(KikooRenderer::Geometry::Ray in,  Point point, glm::vec3& attenuation, KikooRenderer::Geometry::Ray& scattered) {

        if(useBrdf) {
            glm::mat4 worldToTangentMatrix(1);
            worldToTangentMatrix[0] = glm::vec4(glm::normalize(point.tangent), 0);
            worldToTangentMatrix[1] = glm::vec4(glm::normalize(point.bitangent), 0);
            worldToTangentMatrix[2] = glm::vec4(glm::normalize(point.normal), 0);
            worldToTangentMatrix[3] = glm::vec4(0, 0, 0, 1);

            float numSamples = 1;
            glm::vec3 wi = -glm::vec3(worldToTangentMatrix * glm::vec4(in.direction, 0));
            // attenuation = brdf.Sample_f(wi, &t, glm::vec2(1), &numSamples);
            // scattered = Geometry::Ray(point.position, glm::reflect(in.direction, point.normal));
            
            // glm::vec3 t;
            // attenuation = brdf.Sample_f_Transmission(wi, &t, glm::vec2(1), &numSamples);
            // scattered = Geometry::Ray(point.position, glm::vec3(glm::inverse(worldToTangentMatrix) * glm::vec4(t, 1)));
            
            glm::vec3 target = point.position + point.normal + Geometry::randomInSphere();
            scattered = Geometry::Ray(point.position, target);
            attenuation = brdf.OrenNayar(wi, glm::vec3(worldToTangentMatrix * glm::vec4(target, 0)));
            
        } else {
            glm::vec3 target = point.position + point.normal + Geometry::randomInSphere();
            scattered = Geometry::Ray(point.position, target);
            attenuation = albedo;
        }
        
        //Debugging Materials
        // attenuation = point.position;
        // attenuation = glm::vec3(point.uv, 0);
        // attenuation = point.normal;
        // attenuation = point.tangent;
        // attenuation = point.bitangent;
        return true;
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