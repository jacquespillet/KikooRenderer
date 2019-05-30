#include "MainWindow.hpp"

using namespace KikooRenderer;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
    MainWindow mainWindow;
    
    return app.exec();
}