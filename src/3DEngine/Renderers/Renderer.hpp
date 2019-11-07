#pragma once
#include "Util/Common.h"


namespace KikooRenderer {
namespace CoreEngine {
class PostProcess;
class Scene;

enum RENDER_PIPELINE {FORWARD, HDR};

class Renderer {
public:
    Renderer(Scene* scene);
    virtual void Render();
    virtual void Resize(int w, int h);
    virtual void SetGammaCorrection(bool value);
    virtual void SetMSAA(bool value);
    virtual void Destroy();
    virtual void AddPostEffect(PostProcess* postProcess);
    virtual void RemovePostEffect(PostProcess* postProcess);

protected:
    bool isGammaCorrected = false;
    Scene* scene;
};

class ForwardRenderer : public Renderer {
public:
    ForwardRenderer(Scene* scene);
    virtual void Render();
    virtual void SetMSAA(bool value);
    virtual void Destroy();
    virtual void AddPostEffect(PostProcess* postProcess);
    virtual void RemovePostEffect(PostProcess* postProcess);
};

}
}