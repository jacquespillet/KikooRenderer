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
    MixturePdf(Pdf* p1, Pdf* p2) {
        pdfs[0] = p1;
        pdfs[1] = p2;
    }

    //Return the probability that the ray with the given direction has been generated
    virtual float value(const glm::vec3& direction) const {
        float pdf1 = pdfs[0]->value(direction);
        float pdf2 = pdfs[1]->value(direction);
        return 0.5 * pdf1 + 0.5 * pdf2;

    }

    //Generate a a vector going from the origin to a random point on the shape
    virtual glm::vec3 generate() const {
        // return pdfs[1]->generate();
        double rand = Geometry::RandomInRange(0, 1);
        if(rand < 0.5) {
            glm::vec3 val = pdfs[0]->generate();
            // std::cout << "0  " << val << std::endl;
            return val;
        } else {
            glm::vec3 val = pdfs[1]->generate();
            // std::cout << "1  "<< val << std::endl;
            return val;
        }
    }
private:
    Pdf* pdfs[2];
};
}
}
