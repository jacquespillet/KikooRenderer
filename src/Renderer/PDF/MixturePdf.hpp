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
}
}
