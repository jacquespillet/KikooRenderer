#pragma once
#include "Common.h"

namespace KikooRenderer {
    
void EmptyLayout(QLayout* layout);

class CustomSlider : public QHBoxLayout {
	Q_OBJECT
public:
 	QLabel* labelWidget;
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


class LayerController: public QWidget
{
    Q_OBJECT
    public:
        LayerController(uint16_t layerMask = 0xFFFF);
        virtual void mousePressEvent(QMouseEvent *e);
        virtual void mouseReleaseEvent(QMouseEvent *e);
		void SetMask(uint16_t layerMask);
		
    protected:
        bool isClicked = false;
        std::vector<bool> isChecked;
        std::vector<QPushButton*> buttons;
        uint16_t layerMask;

		int numLayers = 8;

	signals:
		void maskChanged(uint16_t layerMask);
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

class Vector4Inspector : public QWidget {
	Q_OBJECT
public:
	Vector4Inspector(std::string label, glm::vec4 value);
	void Setvalue(glm::vec4 value);
	void Setvalue(float x, float y, float z, float w);
	glm::vec4 GetValue();
private:
	QDoubleSpinBox* xSpinBox;
	QDoubleSpinBox* ySpinBox;
	QDoubleSpinBox* zSpinBox;
	QDoubleSpinBox* wSpinBox;
	glm::vec4 vector;
signals: 
	void Modified(glm::vec4 value);	
};

class Vector3Inspector : public QWidget {
	Q_OBJECT
public:
	Vector3Inspector(std::string label, glm::vec3 value);
	void Setvalue(glm::vec3 value);
	void Setvalue(float x, float y, float z, float w);
	glm::vec3 GetValue();
private:
	QDoubleSpinBox* xSpinBox;
	QDoubleSpinBox* ySpinBox;
	QDoubleSpinBox* zSpinBox;
	QDoubleSpinBox* wSpinBox;
	glm::vec3 vector;
signals: 
	void Modified(glm::vec3 value);	
};

class Vector4ArrayInspector : public QWidget {
	Q_OBJECT
public:
	Vector4ArrayInspector(std::string label, std::vector<glm::vec4> values, glm::vec4 defaultVec=glm::vec4(0));
	void SetSize(int size);
	void Setvalue(std::vector<glm::vec4> values);
	std::vector<glm::vec4> GetValue();
	glm::vec4 At();
private:
	QSpinBox* sizeSpinBox;
	std::vector<glm::vec4> vectors;
	glm::vec4 defaultVec;
signals: 
	void Modified(std::vector<glm::vec4> value);	
};


class Vector3ArrayInspector : public QWidget {
	Q_OBJECT
public:
	Vector3ArrayInspector(std::string label, std::vector<glm::vec3> values, glm::vec3 defaultVec=glm::vec3(0));
	void SetSize(int size);
	void Setvalue(std::vector<glm::vec3> values);
	std::vector<glm::vec3> GetValue();
	glm::vec3 At();
private:
	QSpinBox* sizeSpinBox;
	std::vector<glm::vec3> vectors;
	glm::vec3 defaultVec;
signals: 
	void Modified(std::vector<glm::vec3> value);	
};



class DoubleArrayInspector : public QWidget {
	Q_OBJECT
public:
	DoubleArrayInspector(std::string label, std::vector<double> values, double defaultVec=double(0));
	void SetSize(int size);
	void Setvalue(std::vector<double> values);
	std::vector<double> GetValue();
	double At();
private:
	QSpinBox* sizeSpinBox;
	std::vector<double> numbers;
	double defaultVec;
signals: 
	void Modified(std::vector<double> value);	
};


}