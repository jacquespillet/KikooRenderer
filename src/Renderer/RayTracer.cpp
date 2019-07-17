#include "RayTracer.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "Util/Image.hpp"

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
            // if(depth < 50 && closestPoint.material->Scatter(ray, closestPoint, attenuation, scattered)) {
            //     return attenuation * GetColor(scattered, depth+1);
            // }
        } else {                   
             glm::dvec3 direction = glm::normalize(ray.direction);
            double t = 0.5 * direction.y + 1.0;
            glm::dvec3 backgroundColor = (1.0 - t) * glm::dvec3(1, 1, 1) + t * glm::dvec3(0.5, 0.7, 1);
            return backgroundColor;
        }
    }

    void RayTracer::WriteImage() {
        int width = 200;
        int height = 100;
        int numSamples = 100;

        KikooRenderer::Util::FileIO::Image image(width, height); 
        Camera camera;

        Material material(glm::dvec4(0.8, 0.3, 0.3, 1.0));
        // Sphere* sphere = new Sphere(glm::dvec3(0, 0, -1), 0.5, material);
        Sphere* sphere = new Sphere(glm::dvec3(0, 0, -1), 0.5);
        objects.push_back(sphere);
        // Sphere* sphere2 = new Sphere(glm::dvec3(0, -100.5, -1), 100, material);
        Sphere* sphere2 = new Sphere(glm::dvec3(0, -100.5, -1), 100);
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
        }

        image.toPPM("Test.ppm");
        for(int i=0; i<objects.size(); i++) {
            delete objects[i];
        }
    }
}
}