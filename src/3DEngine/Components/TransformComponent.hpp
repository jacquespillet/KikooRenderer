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
	void SetScaleX(float x);
	void SetScaleY(float y);
	void SetScaleZ(float z);

	void SetRotationX(float x);
	void SetRotationY(float y);
	void SetRotationZ(float z);

	void SetPositionX(float x);
	void SetPositionY(float y);
	void SetPositionZ(float z);
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

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        glm::mat4 GetTransRotMatrix(); 
        glm::mat4 GetModelMatrix(); 
        glm::vec3 GetScale(); 
        
        glm::mat4 GetWorldModelMatrix();

        glm::mat4 GetWorldTransRotMatrix(); 
        glm::mat4 GetWorldTransScaleMatrix(); 
        glm::mat4 GetWorldRotationMatrix();

		glm::vec3 GetWorldPosition();
		glm::vec3 GetWorldScale();

		void SetWorldX(float x);
		void SetWorldY(float y);
		void SetWorldZ(float z);

        bool isScreenSize = false;

		TransformInspector* transformInspector;

		QJsonObject ToJSON();
};
}
}