#include "RayTracer.hpp"
#include "Camera.hpp"
#include "Util/Image.hpp"
#include "Geometry/Util.h"
#include "Util/ThreadingUtil.hpp"

#include "3DEngine/Components/BaseMeshes.hpp"

#include "Shapes/Sphere.hpp"
#include "Shapes/Box.hpp"
#include "Shapes/TriangleMesh.hpp"

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
            glm::vec3 emitted = closestPoint.material->emitted();
            if(depth < 1 && closestPoint.material->Scatter(ray, closestPoint, attenuation, scattered)) { // check if ray is scattered && iterations < 50
                return emitted + attenuation * GetColor(scattered, depth+1);
            } else {
                return emitted;
            }
        } else { //Draw background      
            glm::vec3 direction = glm::normalize(ray.direction);
            double t = 0.5 * direction.y + 1.0;
            glm::vec3 backgroundColor = (1.0 - t) * glm::vec3(1, 1, 1) + t * glm::vec3(0.5, 0.7, 1);
            // glm::vec3 backgroundColor = glm::vec3(0);
            return backgroundColor;
        }
    }

    void RayTracer::WriteImage() {
        int width = 300;
        int height = 200;
        int numSamples = 50;

        KikooRenderer::Util::FileIO::Image image(width, height);

        //1. Create the camera 
        glm::vec3 camPos = glm::vec3(0,1, 3);
        glm::vec3 lookAt = glm::vec3(0, 0, 0);
        double distanceToFocus = glm::distance(camPos, lookAt);
        Camera camera(camPos, lookAt, glm::vec3(0, 1, 0), 70, (double)width/(double)height, 0.0001, distanceToFocus, 0, 1);
        

        // Box* box = new Box(glm::vec3(0,0, 0),glm::vec3(1,1, 1), &lb);
        
        std::vector<glm::vec3> vertex;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec4> colors;
        std::vector<int> triangles;
        CoreEngine::GetCubeBuffers(&vertex, &normals, &uv, &colors, &triangles);        


        // //Box1
        {
            Material lb(glm::vec4(0.73));
            TriangleMesh* box = new TriangleMesh(glm::vec3(1, 0, 0), glm::vec3(1), &lb, vertex, normals, uv, triangles);
            objects.push_back(box);
        }  

        // //Bottom
        // {
        //     Material lb(glm::vec4(0.73));
        //     TriangleMesh* box = new TriangleMesh(glm::vec3(0, -0.5, 0), glm::vec3(1, 0.01, 1), &lb, vertex, normals, uv, triangles);
        //     // TriangleMesh* box = new TriangleMesh(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), &lb, vertex, normals, uv, triangles);
        //     objects.push_back(box);
        // }    

        // // //Top
        // {
        //     Material lb(glm::vec4(0.73));
        //     TriangleMesh* box = new TriangleMesh(glm::vec3(0, 0.5, 0), glm::vec3(1, 0.01, 1), &lb, vertex, normals, uv, triangles);
        //     objects.push_back(box);
        // }  
 
        // // //Back
        // {
        //     Material lb(glm::vec4(0.73));
        //     TriangleMesh* box = new TriangleMesh(glm::vec3(0, 0, -0.5), glm::vec3(1, 1, 0.01), &lb, vertex, normals, uv, triangles);
        //     objects.push_back(box);
        // }

        // //Right
        // {
        //     Material lb(glm::vec4(1, 0, 0, 1));
        //     TriangleMesh* box = new TriangleMesh(glm::vec3(0.5, 0, 0), glm::vec3(0.01,1, 1), &lb, vertex, normals, uv, triangles);
        //     objects.push_back(box);
        // }                     

        // //Left
        // {
        //     Material lb(glm::vec4(0, 1, 0, 1));
        //     TriangleMesh* box = new TriangleMesh(glm::vec3(-0.5, 0, 0), glm::vec3(0.01,1, 1), &lb, vertex, normals, uv, triangles);
        //     objects.push_back(box);
        // }   

        // //Top
        // {
        //     Material lb(glm::vec4(0.73));
        //     // lb.emitter = true;
        //     TriangleMesh* box = new TriangleMesh(glm::vec3(0, 0.45, 0), glm::vec3(0.2, 0.01, 0.2), &lb, vertex, normals, uv, triangles);
        //     objects.push_back(box);
        // }   

        // // //Box1
        // {
        //     Material lb(glm::vec4(0.73));
        //     TriangleMesh* box = new TriangleMesh(glm::vec3(0.15, 0, 0.2), glm::vec3(0.25, 0.4, 0.25), &lb, vertex, normals, uv, triangles);
        //     objects.push_back(box);
        // }  

        // // //Box1
        // {
        //     Material lb(glm::vec4(0.73));
        //     TriangleMesh* box = new TriangleMesh(glm::vec3(-0.15, 0, 0.3), glm::vec3(0.25, 0.34, 0.25), &lb, vertex, normals, uv, triangles);
        //     objects.push_back(box);
        // }  

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

            color = glm::min(glm::vec3(1.0f), glm::max(glm::vec3(0.0f), color));


            image.SetPixel(x, height - y - 1, color);
        }), width * height ).Block();

        image.toPPM("Test.ppm");
        for(int i=0; i<objects.size(); i++) {
            delete objects[i];
        }
    }
}
}