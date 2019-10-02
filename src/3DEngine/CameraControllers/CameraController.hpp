#pragma once

#include "Util/Common.h"

namespace KikooRenderer {
namespace CoreEngine {
class CameraScene;

class CameraController {
public: 
    CameraController(CameraScene* camera);

    virtual void OnKeyPressEvent(QKeyEvent *e);
    virtual void OnMousePressEvent(QMouseEvent *e);
    virtual void OnMouseReleaseEvent(QMouseEvent *e);
    virtual void OnMouseMoveEvent(QMouseEvent *e);
    virtual void OnWheelEvent(QWheelEvent *event);
	virtual	void OnKeyReleaseEvent(QKeyEvent *e);
    virtual void OnUpdate();

    virtual void MoveToPos(glm::vec3 position);

private:
    CameraScene* camera;
    bool isRightClicked = false;
    bool isLeftClicked = false;
    bool isMiddleClicked = false;
    
    int previousX;
    int previousY;

    float speedFactor = 10;
    float elapsedTimeMoving = 0;

    bool forwardPressed = false;
    bool backwardPressed = false;
    bool rightPressed = false;
    bool leftPressed = false;
};
}
}