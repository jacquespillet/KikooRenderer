#pragma once
#include "Util/Common.h"
#include "Component.hpp"
#include "3DEngine/Shader.hpp"
#include "3DEngine/Texture.hpp"
#include "3DEngine/Scene.hpp"

namespace KikooRenderer {
namespace CoreEngine {
class MaterialComponent;

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
	FilePicker(std::string label) : QWidget() {
		QHBoxLayout* mainLayout = new QHBoxLayout();

		QLabel* fileLabel = new QLabel(QString::fromStdString(label));
		mainLayout->addWidget(fileLabel);

		fileLineEdit = new QLineEdit();
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

class MaterialInspector : public QGroupBox {
	Q_OBJECT
	public:
		MaterialInspector(MaterialComponent* materialComponent);
		MaterialComponent* materialComponent;
		Scene* scene;

		void Refresh();

};

class MaterialComponent : public Component {
    public:
        MaterialComponent(Object3D* object);
        void OnStart();
        void OnEnable();
        void OnUpdate();
        void OnRender();
        void OnDestroy();
		void Recompute();

		MaterialInspector* GetInspector();
        
        void SetShader(Shader* shader);
        void SetupShaderUniforms(glm::dmat4 modelMatrix, glm::dmat4 viewMatrix, glm::dmat4 projectionMatrix, Scene* scene);

        Shader* shader;

        float influence;

        glm::vec4 albedo;

        Texture albedoTex;
        Texture specularTex;
        Texture normalTex;

		float ambientFactor;
		float diffuseFactor;
		float specularFactor;
		int shininess;

		MaterialInspector* materialInspector;
};
}
}