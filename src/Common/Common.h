#pragma once

//QT
#include <QtCore/QString>
#include <QtCore/QProcess>
#include <QtCore/QObject>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QStyleFactory>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFileSystemModel>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QListView>
#include <QtWidgets/QAbstractItemView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QColorDialog>
#include <QtGui/QStandardItemModel>
#include <QtGui/QKeyEvent>
#include <QtCore/QJsonValue>
#include <QtGui/QDoubleValidator>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QStyleOption>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QComboBox>
#include <QtGui/QPainter>
#include <QtGui/QWheelEvent>
#include <QtCore/QCoreApplication>
#include <Qt3DCore/Qt3DCore>
#include <Qt3DRender/Qt3DRender>
#include <Qt3DRender/QEffect>
#include <Qt3DRender/QTechnique>
#include <Qt3DInput/Qt3DInput>
#include <Qt3DLogic/Qt3DLogic>
#include <Qt3DExtras/Qt3DExtras>
#include <Qt3DAnimation/Qt3DAnimation>
#include <Qt3DRender/QRenderSettings>


//glm
#define GLM_FORCE_INLINE 
//#define GLM_FORCE_SSE2
#define GLM_FORCE_AVX
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_access.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

 //std lib
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string.h>
#include <string>
#include <math.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <cstdio>
#include <limits>
#include <iomanip>
#include <numeric>
#include <random>

#define TWO_PI 6.28318530717958647692529
#define PI 3.1415926535897932384626433
#define DEGTORAD 0.0174533
