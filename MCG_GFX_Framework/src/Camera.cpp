#include "Camera.h"
#include "Tracer.h"
#include <iostream>

namespace MCG
{

	Ray Camera::fireRay(glm::ivec2 pixel_coord)
	{
		float pixel_coordX = ((float)pixel_coord.x / 640) * 2.0f - 1.0f;
		float pixel_coordY = ((float)pixel_coord.y / 480) * 2.0f - 1.0f;

		glm::vec4 nearPoint = glm::vec4(pixel_coordX, pixel_coordY, -1, 1);
		glm::vec4 farPoint = glm::vec4(pixel_coordX, pixel_coordY, 1, 1);

		nearPoint = nearPoint * (glm::inverse(_projMatrix));
		farPoint = farPoint * (glm::inverse(_projMatrix));

		nearPoint = nearPoint / nearPoint.w;
		farPoint = farPoint / farPoint.w;

		nearPoint = nearPoint * (glm::inverse(_viewMatrix));
		farPoint = farPoint * (glm::inverse(_viewMatrix));

		Ray rtn;

		rtn.origin = glm::vec3(nearPoint);
		rtn.dir = glm::normalize(glm::vec3(farPoint - nearPoint));

		return rtn;
	}
}