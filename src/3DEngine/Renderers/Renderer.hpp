#pragma once
#include "Util/Common.h"

namespace KikooRenderer {
namespace CoreEngine {
class Scene;

class Renderer {
public:
    Renderer(Scene* scene);
    virtual void Render();
protected:
    Scene* scene;
};

class ForwardRenderer : public Renderer {
public:
    ForwardRenderer(Scene* scene);
    virtual void Render();
};

}
}