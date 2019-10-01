#include "QtUtil.hpp"

namespace KikooRenderer {
void EmptyLayout(QLayout* layout)  {
	if (!layout) {
		std::cout << "QtUtil:EmptyLayout: layout null" << std::endl;
		return;
	}
	while(layout->count() > 0)
    {
        QLayoutItem *item = layout->takeAt(0);

        QWidget* widget = item->widget();
        if(widget)
        {
            delete widget;
        }
        else
        {
            QLayout * layout = item->layout();
            if (layout)
            {
                EmptyLayout(layout);
            }
            else
            {
                QSpacerItem * si = item->spacerItem();
                if (si)
                {
                    delete si;
                }
            }
        }
        delete item;
    }
}

}