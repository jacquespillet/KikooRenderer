#pragma once

#include "Util/Common.h"

namespace KikooRenderer {
class ConsoleDock;

/*
Main widget for the dockWidget
*/
class ConsoleDockRoot : public QWidget {
	Q_OBJECT
public:
	ConsoleDockRoot();
	ConsoleDock* dockWidget;

	QVBoxLayout* mainLayout;

	QSize sizeHint() const {
		return QSize(width(), 200);
	}
};

/*
DockWidget for the object inspector
*/
class ConsoleDock : public QDockWidget
{
	Q_OBJECT
public:
	ConsoleDock();
    ConsoleDockRoot* rootWidget; 
    std::stringstream* stdOutBuffer;    
};
}