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

        MeshFilterComponent(Object3D* object);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
        

        void LoadFromBuffers(std::vector<glm::dvec3> vertex,
        std::vector<glm::dvec3> normals,
        std::vector<glm::dvec2> uv,
        std::vector<glm::dvec4> colors,
        std::vector<int> triangles);
        void RebuildBuffers();
        void DestroyBuffers();

        GLenum drawingMode;
        int primitiveSize;

        std::vector<Vertex> vertices;
    private: 
        void InitBuffers();

        std::vector<int> triangles;

        unsigned int vertexBuffer;
        unsigned int elementBuffer;
        unsigned int vertexArrayObject;

};
}
}