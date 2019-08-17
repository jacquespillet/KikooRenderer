#pragma once

#include "Util/Common.h"
#include "3DEngine/Object3D.hpp"

namespace KikooRenderer {
    class ObjectDetailRoot;
    //Widget for object "general" parameters
    class ObjectDetails : public QGroupBox {
        Q_OBJECT
    public:
        ObjectDetails(CoreEngine::Object3D* _object, ObjectDetailRoot* _root);

        CoreEngine::Object3D* object;
        ObjectDetailRoot* root;


    signals:
        void InspectorModified();
    };
}