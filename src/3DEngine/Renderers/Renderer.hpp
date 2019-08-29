#pragma once
#include "Util/Common.h"

namespace KikooRenderer {
namespace CoreEngine {
class Scene;

enum RENDER_PIPELINE {FORWARD, HDR};

class Renderer {
public:
    Renderer(Scene* scene);
    virtual void Render();
    virtual void Resize(int w, int h);
    virtual void SetGammaCorrection(bool value);
protected:
    bool isGammaCorrected = false;
    Scene* scene;
};

class ForwardRenderer : public Renderer {
public:
    ForwardRenderer(Scene* scene);
    virtual void Render();
};

}
}