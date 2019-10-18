#include "MainWindow.hpp"

using namespace KikooRenderer;

int main(int argc, char *argv[])
{
	std::stringstream buffer;
	// std::streambuf * old = std::cout.rdbuf(buffer.rdbuf());

	QApplication app(argc, argv);

	// QFile file("resources/Styles/ConsoleStyle/ConsoleStyle.qss");
	// QFile file("resources/Styles/AMOLED/AMOLED.qss");
	// QFile file("resources/Styles/Aqua/Aqua.qss");
	// QFile file("resources/Styles/Dark/QTDark.stylesheet");
	// QFile file("resources/Styles/ElegantDark/ElegantDark.qss");
	// QFile file("resources/Styles/MaterialDark/MaterialDark.qss");
	QFile file("resources/Styles/Orange/darkorange.stylesheet");
	

	file.open(QFile::ReadOnly);
	QString styleSheet = file.readAll();

	app.setStyleSheet(styleSheet);

	MainWindow mainWindow;
	mainWindow.consoleDock->stdOutBuffer = &buffer;



    
    return app.exec();
}