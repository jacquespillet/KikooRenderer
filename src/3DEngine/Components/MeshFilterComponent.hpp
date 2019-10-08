#pragma once
#include "Util/Common.h"
#include "Component.hpp"
#include "3DEngine/Scene.hpp"

namespace KikooRenderer {
namespace CoreEngine {
enum PRIMITIVE_MESH {MODEL_MESH=0, CUBE_MESH=1, SPHERE_MESH=2, CIRCLE_MESH=3, CONE_MESH=4, QUAD_MESH=5, MESH_PRIMITIVE_ENUM_SIZE=6};

class MeshFilterComponent;    
class MeshFilterInspector : public ComponentInspector {
	Q_OBJECT
	public:
		MeshFilterInspector(MeshFilterComponent* meshFilterComponent);
		MeshFilterComponent* meshFilterComponent;
		Scene* scene;

		QVBoxLayout* mainLayout;

		void Refresh();

        static std::string GetByPrimitiveMesh(PRIMITIVE_MESH type);

};


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

        struct InstanceAttribute {
            glm::mat4 modelMatrix;
            glm::vec4 additionalData1;
            glm::vec4 additionalData2;
        };

        MeshFilterComponent(Object3D* object);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
		void Recompute();

        MeshFilterInspector* meshFilterInspector;
        ComponentInspector* GetInspector();



        void LoadFromBuffers(std::vector<glm::vec3> _vertex,
        std::vector<glm::vec3> _normals,
        std::vector<glm::vec2> _uv,
        std::vector<glm::vec4> _colors,
        std::vector<int> _triangles, 
        bool calculateTangents = false);
        
        void RebuildBuffers();
        void DestroyBuffers();

        GLenum drawingMode;
        GLenum polygonMode;
        int primitiveSize;

        std::vector<Vertex> vertices;
        
        PRIMITIVE_MESH meshType;
        std::string modelpath;

        bool shoudRebuildBuffers = false;

        void SetInstanceAttributes(std::vector<InstanceAttribute> modelMatrices);
        bool renderInstanced = false;
        int numInstances = 100;
        std::vector<InstanceAttribute> instanceAttributes;


        QJsonObject jsonObj;
        QJsonObject ToJSON();
        static void FromJSON(QJsonObject json, Object3D* obj);

        
    private: 
        void InitBuffers();
        void CalculateTangents(std::vector<glm::vec4>& tangents, std::vector<glm::vec3> _vertices, std::vector<glm::vec3> _normals, std::vector<glm::vec2> _uv,std::vector<int> _triangles);

        std::vector<int> triangles;

        unsigned int vertexBuffer;
        unsigned int elementBuffer;
        unsigned int vertexArrayObject;

        //For instanced rendering
        unsigned int matricesBuffer;


};
}
}