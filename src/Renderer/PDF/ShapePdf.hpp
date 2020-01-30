#pragma once
#include "Util/Common.h"
#include "Geometry/Ray.hpp"
#include "Geometry/Util.h"
#include "../Shapes/Shape.hpp"
#include "Pdf.hpp"


namespace KikooRenderer {
namespace OfflineRenderer {

class ShapePdf : public Pdf {
public: 
    ShapePdf(Shape* shape, const glm::vec3 origin) : shape(shape), origin(origin) {}

    //Return the probability that the ray with the given direction has been generated
    virtual float value(const glm::vec3& direction) const {
        // return shape->pdfValue(origin, direction);
    }

    //Generate a a vector going from the origin to a random point on the shape
    virtual glm::vec3 generate() const {
        return shape->random(origin);
    }

private:
    glm::vec3 origin;
    Shape* shape;
};
}
}
