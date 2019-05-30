#pragma once
#include "Common/Common.h"
#include "Component.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class MeshFilterComponent : public Component {
    public:

		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 uv;
			unsigned char r, g, b, a; 
		};    

        MeshFilterComponent();
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        

        void LoadFromBuffers(std::vector<glm::dvec3> vertex,
        std::vector<glm::dvec3> normals,
        std::vector<glm::dvec2> uv,
        std::vector<glm::dvec4> colors,
        std::vector<int> triangles);
        void RebuildBuffers();

    private: 
        void InitBuffers();

        std::vector<Vertex> vertices;
        std::vector<int> triangles;

        unsigned int vertexBuffer;
        unsigned int elementBuffer;
        unsigned int vertexArrayObject;

};
}
}