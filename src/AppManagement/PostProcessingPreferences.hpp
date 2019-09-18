#pragma once

#include "Util/Common.h"

namespace KikooRenderer 
{
namespace CoreEngine {
class PostProcess;
class FXAAPostProcess;
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
    CoreEngine::Scene* scene;

};
}
