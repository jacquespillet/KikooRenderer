#pragma once

#include "Util/Common.h"
#include "MainWindow.hpp"

#include "BackgroundPreferences.hpp"
#include "3DUIPreferences.hpp"

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

        BackgroundPreferences* backgroundPreferences;
        UI3DPreferences* ui3DPreferences;

};

class PreferencesWindow: public QMainWindow {
    Q_OBJECT
    protected:
        void showEvent(QShowEvent *ev);
        void closeEvent(QCloseEvent *event);
        QTabWidget *tabWidget;

    public:
        PreferencesWindow(MainWindow* mainWindow);
        MainWindow* mainWindow;
        RenderPreferences* renderPreferences;
        QSize sizeHint(void) const {return QSize(800, 600);}      
};
}