#pragma once
#include "Ray.h"




namespace MCG
{
	class Sphere
	{
	public:

		glm::vec3 shade(Ray ray, glm::vec3 intersect);

		bool intersectTest(Ray ray);

		bool shadowTest(Ray ray, glm::vec3 lightPos, float lightRadius);

		glm::vec3 intersectPos(Ray ray);



		glm::vec3 position;
		glm::vec3 colour;
		float radius;
		float specularStrength;
		int isReflective;
		float reflectivity;
	};
}