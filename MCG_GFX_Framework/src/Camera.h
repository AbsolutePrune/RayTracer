#pragma once
#include <memory>
#include <GLM/glm.hpp>
#include "Ray.h"
#include <array>
#include "MCG_GFX_Lib.h"
#include <iostream>
#include <list>

namespace MCG
{
	struct Camera
	{
		Ray fireRay(glm::ivec2 pixel_coord);


		glm::vec3 cameraPos = glm::vec3(0, 0, 0);

	protected:

		//creates projection and view matrixes for the near and far planes
		glm::mat4 _viewMatrix = glm::mat4(1.0f);// glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3.5f));
		glm::mat4 _projMatrix = glm::perspective(glm::radians(45.0f), (640.0f / 480.0f), 0.1f, 100.0f);
	};
}