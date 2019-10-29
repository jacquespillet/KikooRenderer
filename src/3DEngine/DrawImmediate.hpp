#pragma once


#include "Util/Common.h"

namespace KikooRenderer
{
	
	
namespace CoreEngine
{
	
	
class Scene; 
class Object3D; 
	
class DrawImmediate
{		
	private:
		Scene* scene; 

        Object3D* wireframeCube;
        Object3D* wireframeSphere;
        Object3D* wireframeCapsule;
        Object3D* wireframeCone;
        Object3D* wireframeCylinder;

    public:
		DrawImmediate(Scene* s);
		~DrawImmediate();
		void Init();
		void Destroy();
		
		void DrawWireBox(glm::vec3 position, glm::vec3 rotation, glm::vec3 size, glm::vec4(color));        
		void DrawWireSphere(glm::vec3 position, glm::vec3 rotation, glm::vec3 size, glm::vec4(color));        
		void DrawWireCapsule(glm::vec3 position, glm::vec3 rotation, glm::vec3 size, glm::vec4(color));        
		void DrawWireCone(glm::vec3 position, glm::vec3 rotation, glm::vec3 size, glm::vec4(color));        
		void DrawWireCylinder(glm::vec3 position, glm::vec3 rotation, glm::vec3 size, glm::vec4(color));        
};

}
}