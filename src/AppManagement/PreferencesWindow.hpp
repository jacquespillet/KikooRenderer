#pragma once

#include "Util/Common.h"
#include "MainWindow.hpp"

namespace KikooRenderer 
{
class MainWindow;
class PreferencesWindow;

class RenderPreferences: public QDockWidget {
Q_OBJECT
public:
    RenderPreferences(PreferencesWindow* mainPrefWindow);
    PreferencesWindow* mainPrefWindow;
protected:
	  QWidget* baseWidget; 
      QVBoxLayout* layout;
};

class PreferencesWindow: public QMainWindow {
    Q_OBJECT
	protected:
      void showEvent(QShowEvent *ev);
	  void closeEvent(QCloseEvent *event);

    public:
		PreferencesWindow(MainWindow* mainWindow);
        MainWindow* mainWindow;
        RenderPreferences* renderPreferences;
		QSize sizeHint(void) const {return QSize(800, 600);}      
};
}