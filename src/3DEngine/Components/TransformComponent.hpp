#pragma once 
#include "Util/Common.h"
#include "Component.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class Object3D;
class TransformComponent;
class Scene;

class TransformInspector : public QGroupBox {
	Q_OBJECT
public:
	TransformInspector(TransformComponent* transformComponent);
	TransformComponent* transformComponent;
	Scene* scene;

	QDoubleSpinBox* xPositionSpinBox;
	QDoubleSpinBox* yPositionSpinBox;
	QDoubleSpinBox* zPositionSpinBox;
	QDoubleSpinBox* xRotationSpinBox;
	QDoubleSpinBox* yRotationSpinBox;
	QDoubleSpinBox* zRotationSpinBox;
	QDoubleSpinBox* xScaleSpinBox;
	QDoubleSpinBox* yScaleSpinBox;
	QDoubleSpinBox* zScaleSpinBox;


	void Refresh();
public slots:
	void SetScaleX(double x);
	void SetScaleY(double y);
	void SetScaleZ(double z);

	void SetRotationX(double x);
	void SetRotationY(double y);
	void SetRotationZ(double z);

	void SetPositionX(double x);
	void SetPositionY(double y);
	void SetPositionZ(double z);
};

class TransformComponent : public Component {
    public: 
        TransformComponent(Object3D* object);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
		void Recompute();

		TransformInspector* GetInspector();

        glm::dvec3 position;
        glm::dvec3 rotation;
        glm::dvec3 scale;

        glm::dmat4 GetTransRotMatrix(); 
        glm::dmat4 GetModelMatrix(); 
        glm::dvec3 GetScale(); 
        
        glm::mat4 GetWorldModelMatrix();
        glm::dmat4 GetWorldTransRotMatrix(); 
		glm::dvec3 GetWorldPosition();
		glm::dvec3 GetWorldScale();

		void SetWorldX(double x);
		void SetWorldY(double y);
		void SetWorldZ(double z);

        bool isScreenSize = false;

		TransformInspector* transformInspector;
};
}
}