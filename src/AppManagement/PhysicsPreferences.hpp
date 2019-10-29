#pragma once

#include "Util/Common.h"
#include "PhysicsEngines/Physics.hpp"

namespace KikooRenderer {
class PreferencesWindow;
class PhysicsPreferences: public QWidget {
Q_OBJECT
public:
    //Bullet params
    Physics::Bullet::WORLD_TYPE bulletWorldType;

    PhysicsPreferences(PreferencesWindow* mainPrefWindow);
    PreferencesWindow* mainPrefWindow;    
};
}