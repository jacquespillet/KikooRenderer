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
	CustomSlider(float min, float max, float step, std::string label, float initialValue);

	int GetValue();

	void SetVisible(bool visible);


public slots:
	void OnSpinBoxChanged(double i)
	{
		slider->setValue(i / step);
	}

	void OnSliderChanged(int i)
	{
		double val = (double)i * step;
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
	ColorPicker(std::string label, int r, int g, int b, int a);

signals:
	QColor ColorPicked(QColor color);

};

class TexturePicker : public QWidget {
	Q_OBJECT
public:
	std::string fileName;
	QFileDialog::FileMode fileMode;
	TexturePicker(std::string label, std::string value = "", bool isMultipleFiles = false);
	void dropEvent(QDropEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dragLeaveEvent(QDragLeaveEvent *event);

	void SetFileName(QString fileName) {
		fileLineEdit->setText(fileName);
	}

private:
	QLineEdit* fileLineEdit;
	QLabel* texPreview ;

signals: 
	void FileModified(QString fileName);
	void FilesModified(QStringList fileNames);
};


}