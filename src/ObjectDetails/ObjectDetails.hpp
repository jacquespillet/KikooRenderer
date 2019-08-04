#pragma once

#include "Util/Common.h"
#include "View3D/View3D.hpp"

namespace KikooRenderer {

	class View3D;
	class ObjectDetails : public QDockWidget
	{
		Q_OBJECT
	public:
		 ObjectDetails();

		QWidget* layoutWindow;
		
		View3D* view3D;
		SceneTree* sceneTree;
	};
}