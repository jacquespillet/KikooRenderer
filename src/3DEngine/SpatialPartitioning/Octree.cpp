#include "Octree.hpp"
#include "../Components/BoundingComponent.hpp"
#include "../BaseObjects.hpp"
#include "3DEngine/Scene.hpp"

namespace KikooRenderer {
namespace SpatialPartition {

Octree::Octree() {

}

void Octree::Build(CoreEngine::Scene* scene, std::vector<CoreEngine::Object3D*> objects, int maxLevel) {
	this->maxLevel = maxLevel;
	this->scene = scene;

	Node root;

	double minX = 999999; double minY = 999999; double minZ = 999999;
	double maxX = -999999; double maxY = -999999; double maxZ = -999999;
	for (int i = 0; i < objects.size(); i++) {
		CoreEngine::BoundingBoxComponent* bb = (CoreEngine::BoundingBoxComponent*)(objects[i]->GetComponent("BoundingBox"));
		if (bb != nullptr) {
			root.objects.push_back(objects[i]);
			glm::dvec3 min;
			glm::dvec3 max;
			bb->GetWorldBounds(&min, &max);
			minX = (min.x < minX) ? minX = min.x : minX;
			minY = (min.y < minY) ? minY = min.y : minY;
			minZ = (min.z < minZ) ? minZ = min.z : minZ;

			maxX = (max.x > maxX) ? maxX = max.x : maxX;
			maxY = (max.y > maxY) ? maxY = max.y : maxY;
			maxZ = (max.z > maxZ) ? maxZ = max.z : maxZ;
		}
	}
	glm::dvec3 min = glm::dvec3(minX, minY, minZ);
	glm::dvec3 max = glm::dvec3(maxX, maxY, maxZ);
	glm::dvec3 size = max - min;
	glm::dvec3 center = min + size * 0.5;

	root.level = 0;
	root.position = center;
	root.size = size;

	this->Subdivide(root);
}

void Octree::Subdivide(Node parentNode) {
	if (parentNode.level < this->maxLevel) {
		glm::dvec3 childSize = parentNode.size * 0.5;
		
		for (int x = -1; x <= 1; x+=2) {
			for (int y = -1; y <= 1; y += 2) {
				for (int z = -1; z <= 1; z += 2) {
					Node childNode;
					childNode.position = parentNode.position + glm::dvec3(x, y, z) * childSize * 0.5;
					childNode.size = childSize;
					childNode.parent = &parentNode;
					childNode.level = parentNode.level + 1;
					parentNode.children.push_back(&childNode);

					CoreEngine::Object3D* object = CoreEngine::GetWireFrameBox(scene, glm::to_string(childNode.position), childNode.position, glm::dvec3(0, 0, 0), childSize, glm::dvec4(0, 1, 0, 1));
					object->Enable();
					scene->AddObject(object);

					bool hasObject = false;
					for (int i = 0; i < parentNode.objects.size(); i++) {
						if (isInside(childNode, parentNode.objects[i])) {
							hasObject = true;
							childNode.objects.push_back(parentNode.objects[i]);
							parentNode.objects.erase(parentNode.objects.begin() + i);
						}
					}

					if (hasObject) {
						nodes.push_back(childNode);
						this->Subdivide(childNode);
					}
				}
			}
		}
	}
}

bool Octree:: isInside(Node node, CoreEngine::Object3D* object) {
	CoreEngine::BoundingBoxComponent* bb = (CoreEngine::BoundingBoxComponent*)(object->GetComponent("BoundingBox"));
	
	if (bb != nullptr) {
		glm::dvec3 objectMin;
		glm::dvec3 objectMax;
		bb->GetWorldBounds(&objectMin, &objectMax);

		glm::dvec3 nodeMin = node.position - node.size * 0.5;
		glm::dvec3 nodeMax = node.position + node.size * 0.5;

		bool isInside = false;
		if (nodeMin.x <= objectMin.x && nodeMin.y <= objectMin.y && nodeMin.z <= objectMin.z && nodeMax.x >= objectMax.x && nodeMax.y >= objectMax.y && nodeMax.z >= objectMax.z) isInside = true;
		return isInside;
	}

	return false;
}

}
}