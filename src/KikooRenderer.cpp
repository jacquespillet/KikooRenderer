#include "MainWindow.hpp"

using namespace KikooRenderer;

int main(int argc, char *argv[])
{
	std::stringstream buffer;
	std::streambuf * old = std::cout.rdbuf(buffer.rdbuf());

	QApplication app(argc, argv);

	QFile file("C:\\Users\\Jacques\\Documents\\Boulot\\2019\\Qt BP\\src\\Style\\Dark.qss");
	file.open(QFile::ReadOnly);
	QString styleSheet = file.readAll();

	app.setStyleSheet(styleSheet);

	MainWindow mainWindow;
	mainWindow.consoleDock->stdOutBuffer = &buffer;



    
    return app.exec();
}