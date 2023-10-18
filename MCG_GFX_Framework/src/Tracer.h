#pragma once
#include "Ray.h"
#include <memory>
#include "Sphere.h"
#include <list>

namespace MCG
{
	class Tracer
	{
	public:
		Tracer();

		glm::vec3 traceRay(Ray traced_ray);

		glm::vec3 traceShadow(Ray shadowRay);

		bool shadowCheck(glm::vec3 intersect, std::shared_ptr<Sphere> sphere);

		int length;

		std::list<std::shared_ptr<Sphere> > sphereList;


	protected:

		glm::vec3 lightPos = glm::vec3(500, 0, -500);
		
		float lightRadius = 5;

		glm::vec3 bgColour = glm::vec3(0.3, 0.3, 0.3);

		int sphereCount;

	};
}