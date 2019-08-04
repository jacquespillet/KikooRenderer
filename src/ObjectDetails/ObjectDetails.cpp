#include "ObjectDetails.hpp"

namespace KikooRenderer
{
	ObjectDetails::ObjectDetails() : QDockWidget("Object Details")
	{
		setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea | Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
	}
}