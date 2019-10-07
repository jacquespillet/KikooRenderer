#include "SceneToFile.hpp"

namespace KikooRenderer {
    void SceneToFile(CoreEngine::Scene* scene, std::string fileName){
        QJsonObject sceneJson = scene->ToJSON();
        QJsonDocument saveDoc(sceneJson);
        
        std::ofstream outfile;      
        outfile.open (fileName, std::ofstream::out | std::ofstream::trunc);
        outfile.write (saveDoc.toJson().constData(),saveDoc.toJson().size());
    }

    void SceneFromFile(CoreEngine::Scene* scene, std::string fileName){
        QFile loadFile(QString::fromStdString(fileName));

        if (!loadFile.open(QIODevice::ReadOnly)) {
            qWarning("Couldn't open save file.");
            return;
        }

        QByteArray saveData = loadFile.readAll();

        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

        scene->FromJSON(loadDoc);
    }
}
