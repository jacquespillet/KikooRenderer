#pragma once

#include "Object3D.hpp"


namespace KikooRenderer {

namespace CoreEngine {

    Object3D* GetCube(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, bool physics = false);
    
    Object3D* GetWireFrameBox(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);

    Object3D* GetQuad(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);
    
    Object3D* GetCircle(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);

	Object3D* GetWireCircle(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);

    Object3D* GetSphere(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);

    Object3D* GetWireSphere(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);

    Object3D* GetCapsule(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, float height=2);
    
    Object3D* GetWireCapsule(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, float height=2);
    
    Object3D* GetCone(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, bool isScreenSize=false);

    Object3D* GetLine(Scene* scene, std::string name, glm::vec3 position1, glm::vec3 position2, glm::vec4 color, bool isScreenSize=false);

    Object3D* GetTerrain(Scene* scene, std::string name,glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, float width, float height, int subdivisionsX, int subdivisionsY);

    //UI
    Object3D* GetAxes(Scene* scene, std::string name);

    Object3D* GetGrid(Scene* scene, std::string name);

    Object3D* GetMiniQuad(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);

    Object3D* GetAxesHelper(Scene* scene);



    //Curves    
    Object3D* GetHermiteCurve(Scene* scene, std::string name,glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, glm::vec3 point1, glm::vec3 point2, glm::vec3 tan1, glm::vec3 tan2);
    
    Object3D* GetBezierCurve(Scene* scene, std::string name,glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, glm::vec3 point0, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);

    Object3D* GetCatmutRollSpline(Scene* scene, std::string name,glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);

    Object3D* GetNonUniformBSpline(Scene* scene, std::string name,glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);

    Object3D* GetNURBS(Scene* scene, std::string name,glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);

    Object3D* GetBezierSurface(Scene* scene, std::string name,glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, std::vector<glm::vec4> controlPoints);

	//Lights
	Object3D* GetDirectionalLight(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);

	Object3D* GetPointLight(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);

	Object3D* GetSpotLight(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color, double FOV);

    //Object From Files
    CoreEngine::Object3D* ObjectFromModelFile(Scene* scene, std::string name, std::string filename);

    //Particle System
    Object3D* GetParticleSystem(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);

    //Water Tiles
    Object3D* GetWaterTile_1(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);
    
    Object3D* GetWaterTile_2(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);
    
    Object3D* GetWaterTile_3(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);
    
    Object3D* GetWaterTile_4(Scene* scene, std::string name, glm::vec3 _position, glm::vec3 _rotation, glm::vec3 _scale, glm::vec4 _color);
}
}