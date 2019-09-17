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
    PreferencesWindow* mainPrefWindow;
    CoreEngine::PostProcess* grayScalePost;
    CoreEngine::FXAAPostProcess* fxaaPost;
    CoreEngine::Scene* scene;

};
}
