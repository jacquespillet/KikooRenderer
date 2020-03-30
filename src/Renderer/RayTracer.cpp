#include "RayTracer.hpp"
#include "Camera.hpp"
#include "Util/Image.hpp"
#include "Geometry/Util.h"
#include "Util/ThreadingUtil.hpp"
#include "Util/Sample.hpp"
#include "Util/RandomUtil.hpp"

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


    glm::vec3 RayTracer::GetColor(KikooRenderer::Geometry::Ray ray, int depth, const std::vector<glm::vec2>& brdfSamples, int currentSample) {
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
            glm::vec3 emitted = closestPoint.material->emitted(ray, closestPoint);

            if(glm::length(emitted) > 0) { 
                return emitted;
            }
            
            ScatterRecord scatterRecord;
            bool hasScattered = closestPoint.material->Scatter(ray, closestPoint, scatterRecord);
            if(depth < 30 && hasScattered) { //Secondary rays : Get the scattered direction of the scattered ray
                
                glm::mat4 tangentToWorld(1);
                tangentToWorld[0] = glm::vec4(glm::normalize(closestPoint.tangent), 0);
                tangentToWorld[1] = glm::vec4(glm::normalize(closestPoint.bitangent), 0);
                tangentToWorld[2] = glm::vec4(glm::normalize(closestPoint.normal), 0);
                tangentToWorld[3] = glm::vec4(0, 0, 0, 1);
                glm::mat4 worldToTangent = glm::inverse(tangentToWorld);

                BRDF* brdf = (BRDF*) closestPoint.material->brdf;
                
                float pdf  = 0;
                float brdfVal = 0;
                glm::vec3 outDirection = brdf->Generate(worldToTangent * glm::vec4(ray.direction, 0), closestPoint, &pdf, brdfSamples, currentSample, &brdfVal);
                glm::vec3 outDirection_world = tangentToWorld * glm::vec4(outDirection, 0);
                
                scatteredVector = Geometry::Ray(closestPoint.position, glm::normalize(outDirection_world));

                // glm::vec3 res = scatterRecord.attenuation;
                glm::vec3 res(0);
                if(pdf > 0) {
                    // res = glm::vec3(pdf);
                    res = (emitted + brdfVal * scatterRecord.attenuation * closestPoint.material->ScatterPdf(ray, closestPoint, scatteredVector) * GetColor(scatteredVector, depth+1, brdfSamples, currentSample)) / pdf;
                    // res = (emitted + brdfVal * scatterRecord.attenuation * closestPoint.material->ScatterPdf(ray, closestPoint, scatteredVector) * GetColor(scatteredVector, depth+1, brdfSamples, currentSample));
                    // res = (emitted + scatterRecord.attenuation * closestPoint.material->ScatterPdf(ray, closestPoint, scatteredVector) * GetColor(scatteredVector, depth+1, brdfSamples, currentSample)) / pdf;
                }   
                  
                // glm::vec3 inDirection_tangentSpace = worldToTangent * glm::vec4(ray.direction, 0);
                //WHY ARE THESE TWO DIFFERENT ???
                // glm::vec3 res = outDirection_world;
                // glm::vec3 res = glm::reflect(ray.direction, closestPoint.normal);

                // glm::vec3 t = worldToTangent * glm::vec4(ray.direction, 0);
                // glm::vec3 res = glm::reflect(t, glm::vec3(0, 0, 1));
                // res = tangentToWorld * glm::vec4(res, 0);
 
                // std::cout << "INTERSECTION " << depth << "  "  <<  glm::to_string(closestPoint.position) << " Direction " << glm::to_string(outDirection_world) << "  " << closestPoint.t << std::endl;
                // glm::vec3 res = inDirection_tangentSpace;
                // glm::vec3 res = closestPoint.bitangent;
                // glm::vec3 res = closestPoint.tangent;
                // glm::vec3 res = closestPoint.normal;

                // glm::vec3 res = ray.direction; 
                // glm::vec3 res = closestPoint.normal;
                // glm::vec3 res = inDirection_tangentSpace;
                // // glm::vec3 res(pdf);
                // // std::cout << pdf << std::endl;
                // // pdf = std::max(0.000001f, pdf);
                // glm ::vec3 res = scatterRecord.attenuation;
                
                
                // glm::vec3 res = scatterRecord.attenuation;
                // glm::vec3 res = glm::vec3(pdf);
                // glm::vec3 res = glm::vec3(brdfVal); 
                // glm::vec3 res = closestPoint.;
                // glm::vec3 res = outDirection_world;
                
                //Rendering equation
                // glm::vec3 res = (brdfVal *    GetColor(scatteredVector, depth+1));
                // glm::vec3 res = 
                
                //Things to answer : Why is the PDF that low ? 

                //Generate : gives a ray that is within the BRDF cone
                //Evaluate : how much of the light coming from the out ray goes to the in ray ?
                //PDF : What was the probability of sampling that

                delete scatterRecord.pdf;
                return res;
            } else {
                return emitted; 
            }
        } else { //If we did not hit anything, we hit the sky --> returns the sky color
            // glm::vec3 direction = glm::normalize(ray.direction);
            // double t = 0.5 * direction.y + 1.0;
            // glm::vec3 backgroundColor = (1.0 - t) * glm::vec3(1, 1, 1) + t * glm::vec3(0.5, 0.7, 1);
            // std::cout << "WENT OUT " << std::endl;
            glm::vec3 backgroundColor(0);
            return backgroundColor;
        }
    }

    void RayTracer::WriteImage() {
        int width = 600;
        int height = 500;
        int numSamples = 64;

        KikooRenderer::Util::FileIO::Image image(width, height);

        //1. Create the camera 
        glm::vec3 camPos = glm::vec3(0,6, 12);
        glm::vec3 lookAt = glm::vec3(0, 0.5, 0);
        double distanceToFocus = glm::distance(camPos, lookAt);
        Camera camera(camPos, lookAt, glm::vec3(0, 1, 0), 70, (double)width/(double)height, 0.0001, distanceToFocus, 0, 1);
        
        std::vector<glm::vec3> vertex;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec4> colors;
        std::vector<int> triangles;
        CoreEngine::GetCubeBuffers(&vertex, &normals, &uv, &colors, &triangles);        

        
        std::vector<glm::vec2> pixelSamples(numSamples);
        Util::MultiJitter(pixelSamples, numSamples);
        // Util::BoxFilter(pixelSamples, numSamples);
        
        std::vector<glm::vec2> lensSamples(numSamples);
        Util::MultiJitter(lensSamples, numSamples);
        




        // Light
        {
            Material* lb = new Material(glm::vec4(0.73));
            DiffuseBRDF* db = new DiffuseBRDF(glm::vec3(1, 1, 1));
            MixtureBRDF* mb = new MixtureBRDF();
            mb->AddBRDF(db);
            lb->brdf = mb;
            lb->emitter = true;
            TriangleMesh* box = new TriangleMesh(glm::vec3(0, 9, 0), glm::vec3(4, 0.01, 4), lb, vertex, normals, uv, triangles);
            objects.push_back(box);
        }


        // //Bottom
        {
            Material* lb = new Material(glm::vec4(0.73));
            DiffuseBRDF* db = new DiffuseBRDF(glm::vec3(1, 1, 1));
            ShapeBRDF* sb = new ShapeBRDF(objects[0], this);

            MixtureBRDF* mb = new MixtureBRDF();
            mb->AddBRDF(db);
            // mb->AddBRDF(sb);
            lb->brdf = mb;

            // BRDF* db = new BRDF(glm::vec3(1, 1, 1));
            
            TriangleMesh* box = new TriangleMesh(glm::vec3(0, 0, 0), glm::vec3(10, 0.1, 10), lb, vertex, normals, uv, triangles);
            objects.push_back(box);
        }  

        // //Top
        {
            Material* lb = new Material(glm::vec4(0.73));
            DiffuseBRDF* db = new DiffuseBRDF(glm::vec3(1, 1, 1));
            ShapeBRDF* sb = new ShapeBRDF(objects[0], this);
            MixtureBRDF* mb = new MixtureBRDF();
            mb->AddBRDF(db);
            // mb->AddBRDF(sb);
            lb->brdf = mb;

            
            TriangleMesh* box = new TriangleMesh(glm::vec3(0, 10, 0), glm::vec3(10, 0.1, 10), lb, vertex, normals, uv, triangles);
            objects.push_back(box);
        }

        // //Back
        {
            Material* lb = new Material(glm::vec4(0.73));
            DiffuseBRDF* db = new DiffuseBRDF(glm::vec3(1, 1, 1));
            ShapeBRDF* sb = new ShapeBRDF(objects[0], this);
            MixtureBRDF* mb = new MixtureBRDF();
            mb->AddBRDF(db);
            // mb->AddBRDF(sb);
            lb->brdf = mb;
            
            // lb->emitter = true;
            TriangleMesh* box = new TriangleMesh(glm::vec3(0, 5, -5), glm::vec3(10, 10, 0.1), lb, vertex, normals, uv, triangles);
            objects.push_back(box);
        }

        //Right
        {
            Material* lb = new Material(glm::vec4(0.8, 0, 0, 1));
            DiffuseBRDF* db = new DiffuseBRDF(glm::vec3(1, 1, 1));
            ShapeBRDF* sb = new ShapeBRDF(objects[0], this);
            MixtureBRDF* mb = new MixtureBRDF();
            mb->AddBRDF(db);
            // mb->AddBRDF(sb);
            lb->brdf = mb;
            
            TriangleMesh* box = new TriangleMesh(glm::vec3(5, 5, 0), glm::vec3(0.1,10, 10), lb, vertex, normals, uv, triangles);
            objects.push_back(box);
        }                     

        //Left
        {
            Material* lb = new Material(glm::vec4(0, 0.8, 0, 1));
            DiffuseBRDF* db = new DiffuseBRDF(glm::vec3(1, 1, 1));
            ShapeBRDF* sb = new ShapeBRDF(objects[0], this);
            MixtureBRDF* mb = new MixtureBRDF();
            mb->AddBRDF(db);
            // mb->AddBRDF(sb);
            lb->brdf = mb;
            
            TriangleMesh* box = new TriangleMesh(glm::vec3(-5, 5, 0), glm::vec3(0.1,10, 10), lb, vertex, normals, uv, triangles);
            objects.push_back(box);
        }   


        // Box1
        // {
        //     Material* lb = new Material(glm::vec4( 0.73, 0.73,  0.73, 1));
        //     BRDF* db = new BRDF(glm::vec3(1, 1, 1));
        //     // // DiffuseBRDF* db = new DiffuseBRDF(glm::vec3(1, 1, 1));
        //     // // ShapeBRDF* sb = new ShapeBRDF(objects[0]);
        //     // MixtureBRDF* mb = new MixtureBRDF();
        //     // mb->AddBRDF(db);
        //     // // mb->AddBRDF(sb);
        //     lb->brdf = db;

        //     // TriangleMesh* box = new TriangleMesh(glm::vec3(0, 4, 0), glm::vec3(3), lb, "resources/Models/bunny/untitled.obj");
        //     TriangleMesh* box = new TriangleMesh(glm::vec3(0, 2, 0), glm::vec3(7, 1, 7), lb, vertex, normals, uv, triangles);
        //     objects.push_back(box);
        // }

        // // //Box1
        // {
        //     Material* lb = new Material(glm::vec4(0, 0.73, 0, 1));
        //     // DiffuseBRDF* db = new DiffuseBRDF(glm::vec3(1, 1, 1));
        //     ShapeBRDF* sb = new ShapeBRDF(objects[0]);
        //     // MixtureBRDF* mb = new MixtureBRDF();
        //     // mb->AddBRDF(db);
        //     // mb->AddBRDF(sb);
        //     lb->brdf = sb;
            
        //     TriangleMesh* box = new TriangleMesh(glm::vec3(-2, 0, 0), glm::vec3(2.5, 10, 2.5), lb, vertex, normals, uv, triangles);
        //     // TriangleMesh* box = new TriangleMesh(glm::vec3(-0.2, 0, 0), glm::vec3(0.1, 0.4, 0.2), lb, "resources/Models/bunny/untitled.obj");
        //     objects.push_back(box);
        // }




        clock_t tStart = clock();
        // for(int i=0; i<width * height; i++) {
        KikooRenderer::Util::ThreadPool( std::function<void(uint64_t, uint64_t)>([this, width, numSamples, height, &camera, &image,  pixelSamples](uint64_t i, uint64_t t)
        {
            std::vector<glm::vec2> brdfSamples(numSamples);
            Util::MultiJitter(brdfSamples, numSamples);

            int x = i % width;
            int y = i / width;

            if(x ==0) std::cout << y << " / " << height << std::endl;

            glm::vec3 color(0);

            double numSamplesInv = 1.0 / (double)numSamples;
            for(int j=0; j<numSamples; j++) {
                //For each pixel, we get n samples. Each sample deviates the original ray randomly
                // double randomX = ((double) rand()) / (double) RAND_MAX;
                // double randomY = ((double) rand()) / (double) RAND_MAX;
                int sampleInx = Util::GetRand() * ( pixelSamples.size() -1);
                double randomX = pixelSamples[sampleInx].x;
                double randomY = pixelSamples[sampleInx].y;
                double u = ((double)x + randomX) / (double)width;
                double v = ((double)y + randomY) / (double)height;

                //Get ray object from camera
                KikooRenderer::Geometry::Ray ray =  camera.GetRay(u, v);
                
                //Raytracing
                // glm::vec3 sampleCol = GetColor(ray, 0, brdfSamples, j);

                glm::vec3 sampleCol = ray.direction;
                color += sampleCol * numSamplesInv;
            }
            // if(y > 300)  std::cout << x << "  " << y <<  std::endl;

            //Gamma correction
            color.r = sqrt(color.r);
            color.g = sqrt(color.g);
            color.b = sqrt(color.b);

            color = glm::min(glm::vec3(1.0f), glm::max(glm::vec3(0.0f), color));

            image.SetPixel(x, height - y - 1, color);
        }), width * height).Block();
        // }

        // std::cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << std::endl;
        
        // for(int i=0; i<numSamples; i++) {
        //     double x = pixelSamples[i].x * (width-1);
        //     double y = pixelSamples[i].y * (height-1);

        //     image.SetPixel(x, height - y - 1, glm::vec3(1));
        // }

        image.toPPM("Test.ppm");
        for(int i=0; i<objects.size(); i++) {
            delete objects[i];
        }
    }
}
}