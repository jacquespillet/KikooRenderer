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

Vector4Inspector::Vector4Inspector(std::string label, glm::vec4 value) {

	QVBoxLayout* mainLayout = new QVBoxLayout();

	//Position
	QLabel* positionLabel = new QLabel(QString::fromStdString(label));
	QHBoxLayout* positionLayout = new QHBoxLayout();

	QLabel* xLabel = new QLabel("x"); xSpinBox = new QDoubleSpinBox(); 
	xSpinBox->setDecimals(4); xSpinBox->setRange(-500, 500);  xSpinBox->setValue(value.x);  positionLayout->addWidget(xLabel); positionLayout->addWidget(xSpinBox);
	QLabel* yLabel = new QLabel("y"); ySpinBox = new QDoubleSpinBox(); 
	ySpinBox->setDecimals(4); ySpinBox->setRange(-500, 500); ySpinBox->setValue(value.y); positionLayout->addWidget(yLabel);  positionLayout->addWidget(ySpinBox);
	QLabel* zLabel = new QLabel("z"); zSpinBox = new QDoubleSpinBox(); 
	zSpinBox->setDecimals(4); zSpinBox->setRange(-500, 500); zSpinBox->setValue(value.z); positionLayout->addWidget(zLabel);  positionLayout->addWidget(zSpinBox);
	QLabel* wLabel = new QLabel("w"); wSpinBox = new QDoubleSpinBox(); 
	wSpinBox->setDecimals(4); wSpinBox->setRange(-500, 500); wSpinBox->setValue(value.w); positionLayout->addWidget(wLabel);  positionLayout->addWidget(wSpinBox);

    connect(xSpinBox, static_cast<void (QDoubleSpinBox::*)(double value)>(&QDoubleSpinBox::valueChanged), this, [this, mainLayout](double value) {
        vector.x = value;
        emit Modified(vector);
    });

    connect(ySpinBox, static_cast<void (QDoubleSpinBox::*)(double value)>(&QDoubleSpinBox::valueChanged), this, [this, mainLayout](double value) {
        vector.y = value;
        emit Modified(vector);
    });
    
    connect(zSpinBox, static_cast<void (QDoubleSpinBox::*)(double value)>(&QDoubleSpinBox::valueChanged), this, [this, mainLayout](double value) {
        vector.z = value;
        emit Modified(vector);
    });
    
    connect(wSpinBox, static_cast<void (QDoubleSpinBox::*)(double value)>(&QDoubleSpinBox::valueChanged), this, [this, mainLayout](double value) {
        vector.w = value;
        emit Modified(vector);
    });
    setLayout(positionLayout);
}

void Vector4Inspector::Setvalue(glm::vec4 value) {
    xSpinBox->setValue(value.x);
    ySpinBox->setValue(value.y);
    zSpinBox->setValue(value.z);
    wSpinBox->setValue(value.w);
}
void Vector4Inspector::Setvalue(float x, float y, float z, float w) {
    xSpinBox->setValue(x);
    ySpinBox->setValue(y);
    zSpinBox->setValue(z);
    wSpinBox->setValue(w);
}
glm::vec4 Vector4Inspector::GetValue() {
    return vector;
}

Vector4ArrayInspector::Vector4ArrayInspector(std::string label, std::vector<glm::vec4> values, glm::vec4 defaultVec) {
	this->vectors = values;
    this->defaultVec = defaultVec;

    QVBoxLayout* mainLayout = new QVBoxLayout();

	//Label & Vector size
	QLabel* xLabel = new QLabel("size"); sizeSpinBox = new QSpinBox(); 
	sizeSpinBox->setRange(-500, 500);  sizeSpinBox->setValue(vectors.size());  
    mainLayout->addWidget(xLabel); 
    mainLayout->addWidget(sizeSpinBox);

    //Layout for vector
    QVBoxLayout* vectorsLayout = new QVBoxLayout();
    mainLayout->addLayout(vectorsLayout);

    //Add all vectors
    for(int i=0; i<vectors.size(); i++) {
        Vector4Inspector* vi = new Vector4Inspector(std::to_string(i), vectors[i]);
        vectorsLayout->addWidget(vi);
    }    

    connect(sizeSpinBox, static_cast<void (QSpinBox::*)(int value)>(&QSpinBox::valueChanged), this, [this, vectorsLayout, mainLayout](int size) {
        EmptyLayout(vectorsLayout);
        vectors.resize(size);
        for(int i=0; i<size; i++) {
            Vector4Inspector* vi = new Vector4Inspector(std::to_string(i), vectors[i]);
            vectorsLayout->addWidget(vi);

            connect(vi, &Vector4Inspector::Modified, this, [this, i](glm::vec4 vector) {
                vectors[i] = vector;
                emit Modified(vectors);
            });
        }
        emit Modified(vectors);
    });
    
    setLayout(mainLayout);
}

void Vector4ArrayInspector::SetSize(int size) {

}

void Vector4ArrayInspector::Setvalue(std::vector<glm::vec4> values) {

}

std::vector<glm::vec4> Vector4ArrayInspector::GetValue() {
    std::vector<glm::vec4> res;
    return res;
}

glm::vec4 At() {
    return glm::vec4(0);
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