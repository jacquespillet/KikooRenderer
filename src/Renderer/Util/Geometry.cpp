#include "Geometry.hpp"

namespace KikooRenderer{
namespace OfflineRenderer {

void CalculateTangents(std::vector<glm::vec3>& tangents,std::vector<glm::vec3>& bitangents ,std::vector<glm::vec3> _vertices,  std::vector<glm::vec3> normals, std::vector<glm::vec2> uv,std::vector<int> triangles) {
	std::vector<glm::vec4> tan1(_vertices.size(), glm::vec4(0));
	std::vector<glm::vec4> tan2(_vertices.size(), glm::vec4(0));
	for(uint64_t i=0; i<triangles.size(); i+=3) {
		glm::vec3 v1 = _vertices[triangles[i]];
		glm::vec3 v2 = _vertices[triangles[i + 1]];
		glm::vec3 v3 = _vertices[triangles[i + 2]];

		glm::vec2 w1 = uv[triangles[i]];
		glm::vec2 w2 = uv[triangles[i+1]];
		glm::vec2 w3 = uv[triangles[i+2]];

		double x1 = v2.x - v1.x;
		double x2 = v3.x - v1.x;
		double y1 = v2.y - v1.y;
		double y2 = v3.y - v1.y;
		double z1 = v2.z - v1.z;
		double z2 = v3.z - v1.z;

		double s1 = w2.x - w1.x;
		double s2 = w3.x - w1.x;
		double t1 = w2.y - w1.y;
		double t2 = w3.y - w1.y;

  		double r = 1.0F / (s1 * t2 - s2 * t1);
		glm::vec4 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r, 0);
		glm::vec4 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r, 0);

		tan1[triangles[i]] += sdir;
		tan1[triangles[i + 1]] += sdir;
		tan1[triangles[i + 2]] += sdir;
		
		tan2[triangles[i]] += tdir;
		tan2[triangles[i + 1]] += tdir;
		tan2[triangles[i + 2]] += tdir;

	}
	tangents.resize(_vertices.size());
	bitangents.resize(_vertices.size());
	for(uint64_t i=0; i<_vertices.size(); i++) { 
		glm::vec3 n = normals[i];
		glm::vec3 t = glm::vec3(tan1[i]);

		tangents[i] = glm::vec3(glm::normalize((t - n * glm::dot(n, t))));
		
		float handedness = (glm::dot(glm::cross(n, t), glm::vec3(tan2[i])) < 0.0F) ? -1.0F : 1.0F;
        bitangents[i] = glm::normalize(glm::cross(n, tangents[i]) * handedness); 
	}
}

}
}