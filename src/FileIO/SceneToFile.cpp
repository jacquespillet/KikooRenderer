#include "SceneToFile.hpp"

namespace KikooRenderer {
    void SceneToFile(CoreEngine::Scene* scene, std::string fileName){
        std::cout <<"SceneToFile:SceneToFile: " << fileName << std::endl;
        QJsonObject sceneJson = scene->ToJSON();


        QJsonDocument saveDoc(sceneJson);
        // saveFile.write(saveFormat == Json
        //     ? saveDoc.toJson()
        //     : saveDoc.toBinaryData());
        
        std::ofstream outfile;      
        outfile.open (fileName, std::ofstream::out | std::ofstream::trunc);
        outfile.write (saveDoc.toJson().constData(),saveDoc.toJson().size());
    }

    void SceneFromFile(CoreEngine::Scene* scene, std::string fileName){}
}
