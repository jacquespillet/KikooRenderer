#include "ObjectDetails.hpp"

namespace KikooRenderer
{
	ObjectDetailsPanel::ObjectDetailsPanel() : QDockWidget("Object Details")
	{
		setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);

		mainWidget = new QWidget();
		mainLayout = new QVBoxLayout();

		//ObjectDetails* obj = new ObjectDetails("test");
		
		mainWidget->setLayout(mainLayout);
		setWidget(mainWidget);
	}
}