#pragma once

#include "Util/Common.h"

namespace KikooRenderer {

class PreferencesWindow;

class UI3DPreferences : public QGroupBox {
Q_OBJECT
public:
    UI3DPreferences(PreferencesWindow* mainPrefWindow);
private:
    PreferencesWindow* mainPrefWindow;
};

}