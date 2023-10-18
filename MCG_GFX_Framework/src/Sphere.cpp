#include "Sphere.h"
#include <iostream>

namespace MCG
{


	bool Sphere::intersectTest(Ray ray)
	{
		glm::vec3 n = ray.dir; // n = rays direction
		glm::vec3 a = ray.origin; // a = world origin to ray origin
		glm::vec3 P = position; // p = sphere position
		glm::vec3 Pa = position - a; // P-a = ray origin to sphere centre
		

		float d = glm::length(Pa - glm::dot(Pa, n) * n);
		//float x = sqrt((radius * radius) - (d * d));
		//glm::vec3 firstIntersect = a + (glm::dot(Pa, n) - x) * n;
		//std::cout << d << std::endl;
		if (d <= radius )//&& firstIntersect.z <0)
		{
			if (glm::dot(Pa, n) > 0.0f)
			{
				return true;
			}

		}
		return false;
	}

	bool Sphere::shadowTest(Ray shadowRay, glm::vec3 lightPos, float lightRadius)
	{
		glm::vec3 n = shadowRay.dir; // n = rays direction
		glm::vec3 a = shadowRay.origin; // a = world origin to ray origin
		glm::vec3 P = position; // p = sphere position
		glm::vec3 Pa = position - a; // P-a = ray origin to lightcentre

		glm::vec3 pointX = a + glm::dot(Pa, n) * n;

		glm::vec3 xP = P - pointX;

		float d = glm::length(xP);
		//std::cout << d << std::endl;
		//float d = glm::length(Pa - glm::dot(Pa, n) * n);
		//float x = sqrt((radius * radius) - (d * d));
		//glm::vec3 firstIntersect = a + (glm::dot(Pa, n) - x) * n;

		if (d <= radius)
		{
			std::cout << "true" << std::endl;
			return true;
		}
		else
		{
			return false;
		}
	}

	glm::vec3 Sphere::intersectPos(Ray ray)
	{
		glm::vec3 n = ray.dir; // n = rays direction
		glm::vec3 a = ray.origin; // a = world origin to ray origin
		glm::vec3 P = position; // p = sphere position
		glm::vec3 Pa = position - a; // P-a = ray origin to sphere centre



		float d = glm::length(Pa - glm::dot(Pa, n) * n);

		float x = sqrt((radius * radius) - (d * d));
		glm::vec3 firstIntersect = a + (glm::dot(Pa, n) - x) * n;

		return firstIntersect;
	}

	glm::vec3 Sphere::shade(Ray ray, glm::vec3 intersect)
	{
		glm::vec3 centerToIntersect = intersect - position;

		glm::vec3 lightPos = glm::vec3(500, 0, -500);
		glm::vec3 diffuseColour = glm::vec3(1, 1, 1);
		glm::vec3 specularColour = glm::vec3(1, 1, 1);

		glm::vec3 N = glm::normalize(centerToIntersect);

		glm::vec3 lightDir = normalize(lightPos - intersect);
		float diff = glm::max(glm::dot(N, lightDir), 0.0f);
		glm::vec3 diffuse = diffuseColour * diff;

		glm::vec3 viewDir = glm::normalize(-ray.dir);
		glm::vec3 reflectDir = glm::reflect(-lightDir, N);
		float spec = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), specularStrength);
		glm::vec3 specular = spec * specularColour;

		glm::vec3 lighting = diffuse + specular;
		glm::vec3 shadeColour = glm::vec4(lighting * colour, 1);

		return shadeColour;
	}
}

	


//https://bheisler.github.io/post/writing-raytracer-in-rust-part-3/