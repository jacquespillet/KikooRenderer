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

#include "SpatialAcceleration/BVH.hpp"
#include "Shapes/Aggregate.hpp"

#include "PDF/cosinePdf.hpp"
#include "PDF/ShapePdf.hpp"
#include "PDF/MixturePdf.hpp"

namespace KikooRenderer {
namespace OfflineRenderer {

    RayTracer::RayTracer() {
        
    }
    
    void RayTracer::SetScene() {
        
    }


    glm::vec3 RayTracer::GetColor(KikooRenderer::Geometry::Ray ray, int depth) {
        Point closestPoint = { std::numeric_limits<float>::max(), glm::vec3(0), glm::vec3(0), nullptr, glm::vec2(0),  glm::vec3(0),  glm::vec3(0)};
        bool hit = false;

        //1. Find the closest object that hits the ray ( This is where we should use bounding boxes)
        for(int i=0; i<objects.size(); i++) {
            Point hitPoint;
            double t = objects[i]->HitRay(ray, 0.0001, std::numeric_limits<float>::max(), hitPoint);
            if(t > 0) {
                hit = true;
                if(hitPoint.t < closestPoint.t) {
                    closestPoint = hitPoint;
                }
            }
        }

            
        if(hit) {//If we hit something
            KikooRenderer::Geometry::Ray scatteredVector; //Scattered ray
            glm::vec3 attenuation;
            glm::vec3 emitted = closestPoint.material->emitted(ray, closestPoint);
            
            float pdf;
            bool hasScattered = closestPoint.material->Scatter(ray, closestPoint, attenuation, scatteredVector, pdf);
            if(depth < 10 && hasScattered) { //Secondary rays : Get the scattered direction of the scattered ray
                //SAMPLE LIGHT
                ShapePdf sp(objects[5], closestPoint.position);
                // scatteredVector = Geometry::Ray(closestPoint.position, sp.generate());
                // pdf = sp.value(scatteredVector.direction);
                
                //SAMPLE COS
                glm::mat4 worldToTangent(1);
                worldToTangent[0] = glm::vec4(glm::normalize(closestPoint.tangent), 0);
                worldToTangent[1] = glm::vec4(glm::normalize(closestPoint.bitangent), 0);
                worldToTangent[2] = glm::vec4(glm::normalize(closestPoint.normal), 0);
                worldToTangent[3] = glm::vec4(0, 0, 0, 1);                
                CosinePdf cp(worldToTangent);
                // scatteredVector = Geometry::Ray(closestPoint.position, p.generate());
                // pdf = p.value(scatteredVector.direction);

                MixturePdf mp(&sp, &cp);
                scatteredVector = Geometry::Ray(closestPoint.position, mp.generate());
                pdf = mp.value(scatteredVector.direction);

                glm::vec3 res = emitted + attenuation * closestPoint.material->ScatterPdf(ray, closestPoint, scatteredVector) * GetColor(scatteredVector, depth+1) / pdf; //Get the color of this scattered ray, times it with previous ray attenuation
                return res;
            } else {
                return emitted; //If the ray was not scattered, return the color of the last attenuation
            }
        } else { //If we did not hit anything, we hit the sky --> returns the sky color
            // glm::vec3 direction = glm::normalize(ray.direction);
            // double t = 0.5 * direction.y + 1.0;
            // glm::vec3 backgroundColor = (1.0 - t) * glm::vec3(1, 1, 1) + t * glm::vec3(0.5, 0.7, 1);
            glm::vec3 backgroundColor(0);
            return backgroundColor;
        }
    }

    void RayTracer::WriteImage() {
        int width = 500;
        int height = 450;
        int numSamples = 1000;

        KikooRenderer::Util::FileIO::Image image(width, height);

        //1. Create the camera 
        glm::vec3 camPos = glm::vec3(0,0.5, 1.2);
        glm::vec3 lookAt = glm::vec3(0, 0.5, 0);
        double distanceToFocus = glm::distance(camPos, lookAt);
        Camera camera(camPos, lookAt, glm::vec3(0, 1, 0), 70, (double)width/(double)height, 0.0001, distanceToFocus, 0, 1);
        
        std::vector<glm::vec3> vertex;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec4> colors;
        std::vector<int> triangles;
        CoreEngine::GetCubeBuffers(&vertex, &normals, &uv, &colors, &triangles);        


        // //Bottom
        {
            Material* lb = new Material(glm::vec4(0.73));
            TriangleMesh* box = new TriangleMesh(glm::vec3(0, 0, 0), glm::vec3(1, 0.01, 1), lb, vertex, normals, uv, triangles);
            objects.push_back(box);
        }  

        // //Top
        {
            Material* lb = new Material(glm::vec4(0.73));
            TriangleMesh* box = new TriangleMesh(glm::vec3(0, 1, 0), glm::vec3(1, 0.01, 1), lb, vertex, normals, uv, triangles);
            objects.push_back(box);
        }

        // //Back
        {
            Material* lb = new Material(glm::vec4(0.73));
            TriangleMesh* box = new TriangleMesh(glm::vec3(0, 0.5, -0.5), glm::vec3(1, 1, 0.01), lb, vertex, normals, uv, triangles);
            objects.push_back(box);
        }

        //Right
        {
            Material* lb = new Material(glm::vec4(0.8, 0, 0, 1));
            TriangleMesh* box = new TriangleMesh(glm::vec3(0.5, 0.5, 0), glm::vec3(0.01,1, 1), lb, vertex, normals, uv, triangles);
            objects.push_back(box);
        }                     

        //Left
        {
            Material* lb = new Material(glm::vec4(0, 0.8, 0, 1));
            TriangleMesh* box = new TriangleMesh(glm::vec3(-0.5, 0.5, 0), glm::vec3(0.01,1, 1), lb, vertex, normals, uv, triangles);
            objects.push_back(box);
        }   


        //Light
        {
            Material* lb = new Material(glm::vec4(0.73));
            lb->emitter = true;
            TriangleMesh* box = new TriangleMesh(glm::vec3(0, 0.999, 0), glm::vec3(0.2, 0.01, 0.2), lb, vertex, normals, uv, triangles);
            objects.push_back(box);
        }   

        //Box1
        {
            Material* lb = new Material(glm::vec4(0.73));
            TriangleMesh* box = new TriangleMesh(glm::vec3(0.2, 0, 0), glm::vec3(0.25, 0.6, 0.25), lb, vertex, normals, uv, triangles);
            objects.push_back(box);
        }

        // //Box1
        {
            Material* lb = new Material(glm::vec4(0.73));
            TriangleMesh* box = new TriangleMesh(glm::vec3(-0.2, 0.25, 0), glm::vec3(0.25, 0.6, 0.25), lb, vertex, normals, uv, triangles);
            // TriangleMesh* box = new TriangleMesh(glm::vec3(-0.2, 0, 0), glm::vec3(0.1, 0.4, 0.2), lb, "resources/Models/bunny/untitled.obj");
            objects.push_back(box);
        }




        clock_t tStart = clock();
        KikooRenderer::Util::ThreadPool( std::function<void(uint64_t, uint64_t)>([this, width, numSamples, height, &camera, &image](uint64_t i, uint64_t t)
        {
            int x = i % width;
            int y = i / width;
            if(x ==0) std::cout << y << " / " << height << std::endl;

            glm::vec3 color(0);

            double numSamplesInv = 1.0 / (double)numSamples;
            for(int i=0; i<numSamples; i++) {
                //For each pixel, we get n samples. Each sample deviates the original ray randomly
                double randomX = ((double) rand()) / (double) RAND_MAX;
                double randomY = ((double) rand()) / (double) RAND_MAX;
                double u = (double)(x + randomX) / (double)width;
                double v = (double)(y + randomY) / (double)height;

                //Get ray object from camera
                KikooRenderer::Geometry::Ray ray =  camera.GetRay(u, v);
                
                //Raytracing
                glm::vec3 sampleCol = GetColor(ray, 0);
                color += sampleCol * numSamplesInv;
            }

            //Gamma correction
            color.r = sqrt(color.r);
            color.g = sqrt(color.g);
            color.b = sqrt(color.b);

            color = glm::min(glm::vec3(1.0f), glm::max(glm::vec3(0.0f), color));

            image.SetPixel(x, height - y - 1, color);
        }), width * height).Block();

        std::cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << std::endl;
        
        image.toPPM("Test.ppm");
        for(int i=0; i<objects.size(); i++) {
            delete objects[i];
        }
    }
}
}