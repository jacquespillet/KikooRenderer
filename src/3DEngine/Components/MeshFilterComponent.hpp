#pragma once
#include "Util/Common.h"
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
			glm::vec4 tan;
		};    

        MeshFilterComponent(Object3D* object);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
		void Recompute();
        

        void LoadFromBuffers(std::vector<glm::dvec3> _vertex,
        std::vector<glm::dvec3> _normals,
        std::vector<glm::dvec2> _uv,
        std::vector<glm::dvec4> _colors,
        std::vector<int> _triangles, 
        bool calculateTangents = false);
        
        void RebuildBuffers();
        void DestroyBuffers();

        GLenum drawingMode;
        int primitiveSize;

        std::vector<Vertex> vertices;
    private: 
        void InitBuffers();
        void CalculateTangents(std::vector<glm::vec4>& tangents, std::vector<glm::dvec3> _vertices, std::vector<glm::dvec3> _normals, std::vector<glm::dvec2> _uv,std::vector<int> _triangles);

        std::vector<int> triangles;

        unsigned int vertexBuffer;
        unsigned int elementBuffer;
        unsigned int vertexArrayObject;

};
}
}