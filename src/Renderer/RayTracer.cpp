#include "RayTracer.hpp"
#include "Camera.hpp"
#include "Util/Image.hpp"
#include "Geometry/Util.h"
#include "Util/ThreadingUtil.hpp"

#include "Materials/Material.hpp"
#include "Materials/Lambertian.hpp"
#include "Materials/Metallic.hpp"

namespace KikooRenderer {
namespace OfflineRenderer {

    RayTracer::RayTracer() {
        
    }
    
    void RayTracer::SetScene() {
        
    }


    glm::vec3 RayTracer::GetColor(KikooRenderer::Geometry::Ray ray, int depth) {
        Point closestPoint = {99999999999999999, glm::vec3(0), glm::vec3(0)};
        bool hit = false;

        //1. Find the closest object that hits the ray
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

        
        if(hit) {//If we hit something
            KikooRenderer::Geometry::Ray scattered; //Scattered ray
            glm::vec3 attenuation;
            if(depth < 30 && closestPoint.material->Scatter(ray, closestPoint, attenuation, scattered)) { // check if ray is scattered && iterations < 50
                return attenuation * GetColor(scattered, depth+1);
            } else {
                return glm::vec3(0, 0, 0);
            }
        } else { //Draw background      
             glm::vec3 direction = glm::normalize(ray.direction);
            double t = 0.5 * direction.y + 1.0;
            glm::vec3 backgroundColor = (1.0 - t) * glm::vec3(1, 1, 1) + t * glm::vec3(0.5, 0.7, 1);
            return backgroundColor;
        }
    }

    void RayTracer::WriteImage() {
        int width = 300;
        int height = 200;
        int numSamples = 60;

        KikooRenderer::Util::FileIO::Image image(width, height);

        //1. Create the camera 
        glm::vec3 camPos = glm::vec3(1, 1, 1.5);
        glm::vec3 lookAt = glm::vec3(0, 0, 0);
        double distanceToFocus = glm::distance(camPos, lookAt);
        Camera camera(camPos, lookAt, glm::vec3(0, 1, 0), 90, (double)width/(double)height, 0.0001, distanceToFocus, 0, 1);
        
        //2. Create some objects
        {
            Material material(glm::vec4(0.2, 0.2, 0.2, 1.0));
            Sphere* sphere = new Sphere(glm::vec3(0, -1000, 0), 1000, &material);
            objects.push_back(sphere);
        }

        for(float x = -11; x<11; x+=2) {
            for(float z = -11; z<11; z+=2) {
                double materialRandom = ((double) rand()) / (double) RAND_MAX;   
                double xPos = x + (((double) rand()) / (double) RAND_MAX) * 0.9;   
                double zPos = z + (((double) rand()) / (double) RAND_MAX) * 0.9;   
                double radius = 0.2;
                double r = ((double) rand()) / (double) RAND_MAX;   
                double g = ((double) rand()) / (double) RAND_MAX;   
                double b = ((double) rand()) / (double) RAND_MAX;
                if(materialRandom < 0.6) { //Dielectric
                    double fuzz = ((double) rand()) / (double) RAND_MAX + 1.0;
                    // Material material(glm::vec4(r, g, b, 0.5));
                    Metallic lb(glm::vec4(r, g, b, 0.5));
                    lb.fuzz = 0.01; 
                    Sphere* sphere = new Sphere(glm::vec3(xPos, 0.2, zPos), radius, &lb);
                    objects.push_back(sphere);
                } else if(materialRandom < 0.9) { // Metallic
                    double fuzz = ((double) rand()) / (double) RAND_MAX;
                    Material material(glm::vec4(r, g, b, 1.0));
                    Sphere* sphere = new Sphere(glm::vec3(xPos, 0.2, zPos), radius, &material);
                    objects.push_back(sphere);
                } else { //Diffuse
                    Material material(glm::vec4(r, g, b, 1.0));
                    Sphere* sphere = new Sphere(glm::vec3(xPos, 0.2, zPos), radius, &material);
                    objects.push_back(sphere);
                }
            }
        }

        Material material(glm::vec4(0.7,0.6,0.5, 1.0));
        Sphere* sphere = new Sphere(glm::vec3(0,1, 0), 1.0, &material);
        objects.push_back(sphere);

        Material material2(glm::vec4(0,1,0, 0.5));
        Sphere* sphere2 = new Sphere(glm::vec3(-4,1, 0), 1.0, &material2);
        objects.push_back(sphere2);

        KikooRenderer::Util::ThreadPool( std::function<void(uint64_t, uint64_t)>([this, width, numSamples, height, &camera, &image](uint64_t i, uint64_t t)
        {
            int x = i % width;
            int y = i / width;
            if(x ==0) std::cout << y << " / " << width << std::endl;

            glm::vec3 color(0);

            for(int i=0; i<numSamples; i++) {
                //For each pixel, we get n samples. Each sample deviates the original ray randomly
                double randomX = ((double) rand()) / (double) RAND_MAX;
                double randomY = ((double) rand()) / (double) RAND_MAX;
                double u = (double)(x + randomX) / (double)width;
                double v = (double)(y + randomY) / (double)height;

                //Get ray object from camera
                KikooRenderer::Geometry::Ray ray =  camera.GetRay(u, v);
                
                //Raytracing
                color += GetColor(ray, 0);
            }
            color /= numSamples;

            //Gamma correction
            color.r = sqrt(color.r);
            color.g = sqrt(color.g);
            color.b = sqrt(color.b);


            image.SetPixel(x, height - y - 1, color);
        }), width * height ).Block();

        image.toPPM("Test.ppm");
        for(int i=0; i<objects.size(); i++) {
            delete objects[i];
        }
    }
}
}