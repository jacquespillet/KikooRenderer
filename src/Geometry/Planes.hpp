#pragma once

#include "Util/Common.h"

namespace KikooRenderer {
namespace Geometry {

	struct Planes {
		glm::vec4 rightPlane;
		glm::vec4 leftPlane;
		glm::vec4 topPlane;
		glm::vec4 bottomPlane;
		glm::vec4 nearPlane;
		glm::vec4 farPlane;
	};
}
}
