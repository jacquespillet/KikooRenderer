#include "ConsoleDock.hpp"
#include "3DEngine/Scene.hpp"

namespace KikooRenderer
{	
ConsoleDockRoot::ConsoleDockRoot() : QWidget() {
    mainLayout = new QVBoxLayout();	
    mainLayout->setAlignment(Qt::AlignTop);

    QTextEdit* consoleTextEdit = new QTextEdit();
	consoleTextEdit->setReadOnly(true);    


   	QToolBar *consoleToolbar = new QToolBar(tr("Console"));
	consoleToolbar->setStyleSheet("QToolBar{spacing:-10px;}");

    QIcon clearIcon(("resources/UI/Widgets/ClearConsole.png"));
    QAction *clearAct = new QAction(clearIcon, tr("&clear"), this);
	clearAct->setCheckable(true);
	consoleToolbar->addAction(clearAct);

	connect(clearAct, &QAction::toggled, this, [this, clearAct,consoleTextEdit](){
		clearAct->setChecked(false);
        consoleTextEdit->clear();
	});

    mainLayout->addWidget(consoleToolbar);

    // consoleToolbar->addSeparator();
 
    // layout->addWidget(consoleToolbar);    	




    mainLayout->addWidget(consoleTextEdit);

    QTimer* timer = new QTimer();
    connect(timer, &QTimer::timeout, [this, consoleTextEdit]() {
        if(dockWidget->stdOutBuffer != nullptr) {
            uint64_t pos = dockWidget->stdOutBuffer->tellg();
            dockWidget->stdOutBuffer->seekg (0, dockWidget->stdOutBuffer->end);
            uint64_t end = dockWidget->stdOutBuffer->tellg();            
            uint64_t bytesToRead = end - pos;
            if(bytesToRead > 0) {
                dockWidget->stdOutBuffer->seekg(pos, dockWidget->stdOutBuffer->beg);
                char * buffer = new char [bytesToRead];
                (*dockWidget->stdOutBuffer).read(buffer, bytesToRead);
                consoleTextEdit->append(QString::fromStdString(buffer));
            }
        }
    });
    timer->start(0);
    setLayout(mainLayout);
}
/*
Constructor of the right panel
*/
ConsoleDock::ConsoleDock() : QDockWidget("Console")
{
    setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
    
    rootWidget = new ConsoleDockRoot();
    rootWidget->dockWidget = this;

    setWidget(rootWidget);
}

}