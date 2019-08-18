#pragma once
#include "Common.h"

namespace KikooRenderer {
    
void EmptyLayout(QLayout* layout);

class CustomSlider : public QHBoxLayout {
	Q_OBJECT
public:
	QSlider* slider;
	QDoubleSpinBox* spinBox;
	float step;
	CustomSlider(float min, float max, float step, std::string label, float initialValue) : QHBoxLayout() {
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

		connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(OnSpinBoxChanged(double)));
		addWidget(spinBox);
	}

	int GetValue() {
		double value = spinBox->value();
		return 0;
	}

public slots:
	void OnSpinBoxChanged(double i)
	{
		slider->setValue(i / step);
	}

	void OnSliderChanged(int i)
	{
		double val = (double)i * step;
		std::cout << i << " " << val << std::endl;
		spinBox->setValue(val);
		emit Modified(val);
	}

signals:
	void Modified(double val);
};

class ColorPicker : public QWidget {
	Q_OBJECT
public:
	QColor color;
	ColorPicker(std::string label, int r, int g, int b, int a) : QWidget() {
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

		connect(button, &QPushButton::clicked, this, [this, colorDialog]() {
			colorDialog->exec();
		});


		connect(colorDialog, &QColorDialog::colorSelected, this, [this, colorDialog](const QColor &_color) {
			color = _color;
			emit ColorPicked(color);
		});

		setLayout(mainLayout);
	}
signals:
	QColor ColorPicked(QColor color);

};

class FilePicker : public QWidget {
	Q_OBJECT
public:
	std::string fileName;
	QLineEdit* fileLineEdit;
	FilePicker(std::string label, std::string value = "") : QWidget() {
		QHBoxLayout* mainLayout = new QHBoxLayout();

		QLabel* fileLabel = new QLabel(QString::fromStdString(label));
		mainLayout->addWidget(fileLabel);

		fileLineEdit = new QLineEdit(QString::fromStdString(value));
		mainLayout->addWidget(fileLineEdit);

		QPushButton* button = new QPushButton("Choose File");
		mainLayout->addWidget(button);

		connect(button, &QPushButton::clicked, this, [this]() {
			QFileDialog dialog(this);
			dialog.setFileMode(QFileDialog::ExistingFile);
			dialog.setNameFilter(tr(""));
			dialog.setViewMode(QFileDialog::Detail);
			QStringList fileNames;
			if (dialog.exec())
				fileNames = dialog.selectedFiles();

			if (fileNames.size() > 0) {
				fileLineEdit->setText(fileNames[0]);
				emit FileModified(fileNames[0]);
			}


		});

		setLayout(mainLayout);
	}

	void SetFileName(QString fileName) {
		fileLineEdit->setText(fileName);
	}

signals: 
	void FileModified(QString fileName);
};


}