#pragma once

#include "Util/Common.h"

namespace KikooRenderer {
namespace CoreEngine {
class Scene;
}
class PreferencesWindow;
class RenderingPreferences: public QWidget {
Q_OBJECT
public:
    RenderingPreferences(PreferencesWindow* mainPrefWindow);
    PreferencesWindow* mainPrefWindow;   

    CoreEngine::Scene* scene; 
};
}