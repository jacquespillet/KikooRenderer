#pragma once

#include "Util/Common.h"

namespace KikooRenderer 
{
namespace CoreEngine {
class PostProcess;
class FXAAPostProcess;
class DepthOfFieldPostProcess;
class BloomPostProcess;
class MotionBlurPostProcess;
class Scene;
} 
class PreferencesWindow;
class PostProcessingPreferences: public QWidget {
Q_OBJECT
public:
    PostProcessingPreferences(PreferencesWindow* mainPrefWindow);

private:
    bool isFxaa = false;
    
    PreferencesWindow* mainPrefWindow;
    CoreEngine::PostProcess* grayScalePost;
    CoreEngine::FXAAPostProcess* fxaaPost;
    CoreEngine::DepthOfFieldPostProcess* dofPost;
    CoreEngine::BloomPostProcess* bloomPost;
    CoreEngine::MotionBlurPostProcess* motionBlurPost;
    CoreEngine::Scene* scene;

};
}
