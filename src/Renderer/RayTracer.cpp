#include "RayTracer.hpp"
#include "Camera.hpp"
#include "3DEngine/CameraScene.hpp"
#include "Material.hpp"
#include "Util/Image.hpp"
#include "Geometry/Util.h"

namespace KikooRenderer {
namespace Renderer {

    RayTracer::RayTracer() {
        
    }
    
    void RayTracer::SetScene() {
        
    }


    glm::dvec3 RayTracer::GetColor(KikooRenderer::Geometry::Ray ray, int depth) {
        Point closestPoint = {99999999999999999, glm::dvec3(0), glm::dvec3(0)};
        bool hit = false;
        for(int i=0; i<objects.size(); i++) {
            Point hitPoint;
            double t = objects[i]->HitRay(ray, 0.0001, 999999, hitPoint);
            if(t > 0) {
                hit = true;
                if(hitPoint.t < closestPoint.t) {
                    closestPoint = hitPoint;
                }
            }
        }

        if(hit) {
            KikooRenderer::Geometry::Ray scattered;
            glm::dvec3 attenuation;
            if(depth < 50 && closestPoint.material->Scatter(ray, closestPoint, attenuation, scattered)) {
                return attenuation * GetColor(scattered, depth+1);
            } else {
                return glm::dvec3(0, 0, 0);
            }
        } else {                   
             glm::dvec3 direction = glm::normalize(ray.direction);
            double t = 0.5 * direction.y + 1.0;
            glm::dvec3 backgroundColor = (1.0 - t) * glm::dvec3(1, 1, 1) + t * glm::dvec3(0.5, 0.7, 1);
            return backgroundColor;
        }
    }

    void RayTracer::WriteImage() {
        int width = 1000;
        int height = 800;
        int numSamples = 100;

        KikooRenderer::Util::FileIO::Image image(width, height); 
        glm::dvec3 camPos = glm::dvec3(1, 1, 1.5);
        glm::dvec3 lookAt = glm::dvec3(0, 0, 0);
        double distanceToFocus = glm::distance(camPos, lookAt);
        Camera camera(camPos, lookAt, glm::dvec3(0, 1, 0), 90, (double)width/(double)height, 0.0001, distanceToFocus);
        
        {
            Material material(glm::dvec4(0.2, 0.2, 0.2, 1.0), false);
            Sphere* sphere = new Sphere(glm::dvec3(0, -1000, 0), 1000, material);
            objects.push_back(sphere);
        }

        for(float x = -11; x<11; x++) {
            for(float z = -11; z<11; z++) {
                double materialRandom = ((double) rand()) / (double) RAND_MAX;   
                double xPos = x + (((double) rand()) / (double) RAND_MAX) * 0.9;   
                double zPos = z + (((double) rand()) / (double) RAND_MAX) * 0.9;   
                double radius = 0.2;
                double r = ((double) rand()) / (double) RAND_MAX;   
                double g = ((double) rand()) / (double) RAND_MAX;   
                double b = ((double) rand()) / (double) RAND_MAX;
                if(materialRandom < 0.2) { //Dielectric
                    double refInx = ((double) rand()) / (double) RAND_MAX + 1.0;
                    Material material(glm::dvec4(r, g, b, 0.5), false);
                    material.refInx = refInx; 
                    Sphere* sphere = new Sphere(glm::dvec3(xPos, 0.2, zPos), radius, material);
                    objects.push_back(sphere);
                } else if(materialRandom < 0.5) { // Metallic
                    double fuzz = ((double) rand()) / (double) RAND_MAX;
                    Material material(glm::dvec4(r, g, b, 1.0), true);
                    material.fuzz = fuzz;
                    Sphere* sphere = new Sphere(glm::dvec3(xPos, 0.2, zPos), radius, material);
                    objects.push_back(sphere);
                } else { //Diffuse
                    Material material(glm::dvec4(r, g, b, 1.0), false);
                    Sphere* sphere = new Sphere(glm::dvec3(xPos, 0.2, zPos), radius, material);
                    objects.push_back(sphere);
                }
            }
        }

        Material material(glm::dvec4(0.7,0.6,0.5, 1.0), true);
        material.fuzz = 0.0;
        Sphere* sphere = new Sphere(glm::dvec3(0,1, 0), 1.0, material);
        objects.push_back(sphere);

        Material material2(glm::dvec4(0,1,0, 0.5), true);
        material2.refInx = 1.5;
        Sphere* sphere2 = new Sphere(glm::dvec3(-4,1, 0), 1.0, material2);
        objects.push_back(sphere2);


        for(int y=0; y<height; y++) {
            for(int x=0; x<width; x++) {
                glm::dvec3 color(0);

                for(int i=0; i<numSamples; i++) {
                    double randomX = ((double) rand()) / (double) RAND_MAX;
                    double randomY = ((double) rand()) / (double) RAND_MAX;
                    double u = (double)(x + randomX) / (double)width;
                    double v = (double)(y + randomY) / (double)height;
                    KikooRenderer::Geometry::Ray ray =  camera.GetRay(u, v);

                    color += GetColor(ray, 0);
                }
                color /= numSamples;

                color.r = sqrt(color.r);
                color.g = sqrt(color.g);
                color.b = sqrt(color.b);


                image.SetPixel(x, height - y - 1, color);
            }
            std::cout  << " y " << y << std::endl;
        }

        image.toPPM("Test.ppm");
        for(int i=0; i<objects.size(); i++) {
            delete objects[i];
        }
    }
}
}