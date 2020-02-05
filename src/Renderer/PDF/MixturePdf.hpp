#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"
#include "Geometry/Util.h"
#include "../Shapes/Shape.hpp"
#include "Pdf.hpp"


namespace KikooRenderer {
namespace OfflineRenderer {

class MixturePdf : public Pdf{
public: 
    MixturePdf() {}

    void AddPdf(Pdf* pdf) {
        pdfs.push_back(pdf);
    }

    //Return the probability that the ray with the given direction has been generated
    virtual float value(const glm::vec3& direction) const {
        float totalPdf = 0;
        float pdfWeight = 1 / (float)pdfs.size();
        for(int i=0; i<pdfs.size(); i++) {
            totalPdf += pdfWeight * pdfs[i]->value(direction);
        }
        return totalPdf;
    }

    //Generate a a vector going from the origin to a random point on the shape
    virtual glm::vec3 generate() const {
        int rand = (int)Geometry::RandomInRange(0, pdfs.size());
        return pdfs[rand]->generate();
    }
private:
    std::vector<Pdf*> pdfs;
};

class MixtureBRDF : public BRDF {
public: 
    MixtureBRDF() : BRDF(glm::vec3(1)) {}

    void AddBRDF(BRDF* brdf) {
        brdfs.push_back(brdf);
    }

    ~MixtureBRDF() {
        for(int i=0; i<brdfs.size(); i++) {
            delete brdfs[i];
        }
    }

    //Generate a vector from the in vector
    virtual glm::vec3 Generate(glm::vec3 in, Point pt, float* pdf, const std::vector<glm::vec2>& brdfSamples, int currentSample, float* brdfValue=nullptr) {
        //In is already in tangent space.
        int rand = (int)Geometry::RandomInRange(0, brdfs.size());

        float totalpdf = 0;

        glm::vec3 direction = brdfs[rand]->Generate(in, pt, &totalpdf, brdfSamples, currentSample, brdfValue);
        glm::vec3 wh = glm::normalize(-in + direction);
        
        // *brdf = 0.5 * brdfs[0]->Evaluate(-in, direction) + 0.5 * brdfs[1]->Evaluate(-in, direction);

        float brdfWeight = 1 / (float)brdfs.size();
        totalpdf *= brdfWeight;

        for(int i=0; i<brdfs.size(); i++) {
            if(i != rand) {
                totalpdf += brdfWeight * brdfs[i]->PDF(direction, wh, &pt);
            }
        }

        *pdf = totalpdf;

        
        return direction;
    }
private:
    std::vector<BRDF*> brdfs;
};

}
}
