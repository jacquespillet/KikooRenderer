#include "QtUtil.hpp"

namespace KikooRenderer {

//CustomSlider class
//____________________________________________________________
//
CustomSlider::CustomSlider(float min, float max, float step, std::string label, float initialValue) : QHBoxLayout() {
    this->step = step;

    slider = new QSlider(Qt::Horizontal);
    // slider->setFocusPolicy(Qt::StrongFocus);
    // slider->setTickPosition(QSlider::TicksBothSides);
    // slider->setTickInterval(10);
    slider->setMinimum(min / step);
    slider->setMaximum(max / step);
    slider->setSingleStep(1);
    slider->setValue(initialValue/ step);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(OnSliderChanged(int)));
    addWidget(slider);
    QLabel* labelWidget = new QLabel(QString::fromStdString(label));
    addWidget(labelWidget);

    spinBox = new QDoubleSpinBox();
    spinBox->setMinimum(min);
    spinBox->setMaximum(max);
    spinBox->setSingleStep(step);
    spinBox->setValue(initialValue);

    // connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(OnSpinBoxChanged(double)));
    addWidget(spinBox);
}

int CustomSlider::GetValue() {
    double value = spinBox->value();
    return 0;
}

void CustomSlider::SetVisible(bool visible) {
    slider->setVisible(visible);
    spinBox->setVisible(visible);
}


//ColorPicker class
//____________________________________________________________
//
ColorPicker::ColorPicker(std::string label, int r, int g, int b, int a) : QWidget() {
    QHBoxLayout* mainLayout = new QHBoxLayout();

    QLabel* labelWidget = new QLabel(QString::fromStdString(label));
    mainLayout->addWidget(labelWidget);

    QPushButton* button = new QPushButton("");
    QPalette pal = button->palette();
    button->setFlat(true);
    pal.setColor(QPalette::Button, QColor(r, g, b, a));
    button->setAutoFillBackground(true);
    button->setPalette(pal);
    button->update();
    mainLayout->addWidget(button);

    QColorDialog* colorDialog = new QColorDialog();
    colorDialog->setOptions(QColorDialog::ShowAlphaChannel);

    connect(button, &QPushButton::clicked, this, [this, colorDialog]() {
        colorDialog->exec();
    });


    connect(colorDialog, &QColorDialog::colorSelected, this, [this, colorDialog](const QColor &_color) {
        color = _color;
        emit ColorPicked(color);
    });

    setLayout(mainLayout);
}


//TexturePicker class
//____________________________________________________________
//
TexturePicker::TexturePicker(std::string label, std::string value  , bool isMultipleFiles ) : QWidget() {
    setAcceptDrops(true);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    
    QLabel* fileLabel = new QLabel(QString::fromStdString(label));
    mainLayout->addWidget(fileLabel);
    
    QHBoxLayout* mainHorizLayout = new QHBoxLayout();

    texPreview = new QLabel();
    texPreview->setPixmap(QPixmap().scaled(50, 50));
    mainHorizLayout->addWidget(texPreview);

    fileLineEdit = new QLineEdit(QString::fromStdString(value));
    fileLineEdit->setAcceptDrops(true);
    mainHorizLayout->addWidget(fileLineEdit);

    QPushButton* button = new QPushButton("Choose File");
    mainHorizLayout->addWidget(button);

    fileMode = isMultipleFiles ? QFileDialog::FileMode::ExistingFile : QFileDialog::FileMode::ExistingFiles; 

    connect(button, &QPushButton::clicked, this, [this]() {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::FileMode::ExistingFiles);
        dialog.setNameFilter(tr(""));
        dialog.setViewMode(QFileDialog::Detail);
        QStringList fileNames;
        if (dialog.exec())
            fileNames = dialog.selectedFiles();

        if (fileNames.size() == 1) {
            fileLineEdit->setText(fileNames[0]);
        }
        
        if (fileNames.size() > 1) {
            fileLineEdit->setText(fileNames[0]);
        }
    });

    connect(fileLineEdit, &QLineEdit::textChanged, this, [this](QString text) {
        texPreview->setPixmap(QPixmap(text).scaled(50, 50));
        emit FileModified(text);
    });

    mainLayout->addLayout(mainHorizLayout);

    setLayout(mainLayout);
}

void TexturePicker::dropEvent(QDropEvent *event)
{
    QString texPath = event->mimeData()->text();
    texPath.remove(0, 8);
    fileLineEdit->setText(texPath);
}

void TexturePicker::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void TexturePicker::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}


void TexturePicker::dragMoveEvent(QDragMoveEvent *event) {
    event->acceptProposedAction();
}

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