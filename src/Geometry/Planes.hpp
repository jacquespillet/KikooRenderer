#pragma once

#include "Common/Common.h"

namespace KikooRenderer {
namespace Geometry {

	struct Planes {
		glm::dvec4 rightPlane;
		glm::dvec4 leftPlane;
		glm::dvec4 topPlane;
		glm::dvec4 bottomPlane;
		glm::dvec4 nearPlane;
		glm::dvec4 farPlane;
	};
}
}
