#include "Tracer.h"
#include "Camera.h"
#include <iostream>

//#include "Sphere.h"

namespace MCG
{
	Tracer::Tracer()
	{
		std::cout << "how many spheres?" << std::endl;
		std::cin >> sphereCount;
		//sphereCount = 300;


		for (size_t i = 0; i < sphereCount; i++)
		{
			std::shared_ptr<MCG::Sphere> sphere = std::make_shared<MCG::Sphere>();

			sphere->position = glm::vec3(std::rand() % 150 + -75, std::rand() % 150 + -75, std::rand() % -150 + -800);
			sphere->colour = glm::vec3(static_cast <float> (std::rand() % 100) / 100, static_cast <float> (std::rand() % 100) / 100, static_cast <float> (std::rand() % 100) / 100);
			sphere->radius = std::rand() % 10;
			sphere->specularStrength = std::rand() % 300;
			sphere->isReflective = std::rand() % 2 + 0;

			if (sphere->isReflective)
			{
				sphere->reflectivity = static_cast <float> (std::rand() % 80) / 100;
			}

			sphereList.push_front(sphere);
		}
		std::shared_ptr<MCG::Sphere> sphere1 = std::make_shared<MCG::Sphere>();
		sphere1->position = glm::vec3(20,0,-700);
		sphere1->colour = glm::vec3(1,0,0);
		sphere1->radius =20;
		sphere1->specularStrength = std::rand() % 300;
		sphere1->isReflective = true;//std::rand() % 2 + 0;

		if (sphere1->isReflective)
		{
			sphere1->reflectivity = 0.3;
		}
		else
		{
			sphere1->reflectivity = 1;
		}

		sphereList.push_front(sphere1);

		std::shared_ptr<MCG::Sphere> sphere2 = std::make_shared<MCG::Sphere>();

		sphere2->position = glm::vec3(-20, 0, -700);
		sphere2->colour = glm::vec3(0,0,1);
		sphere2->radius = 20;
		sphere2->specularStrength = std::rand() % 300;
		sphere2->isReflective = false;
		if (sphere2->isReflective)
		{
			sphere2->reflectivity = 0.7;// static_cast <float> (std::rand() % 100) / 100;
		}
		else
		{
			sphere2->reflectivity = 1;
		}

		sphereList.push_front(sphere2);

		length = sphereList.size();
	}

	// once per pixel
	glm::vec3 Tracer::traceRay(Ray traced_ray)
	{
		//maybe leave this blank
		glm::vec3 colour = glm::vec3(0, 0, 0);

		float shortestRay = 6999.0f;
		float rayLength = 10000.0f;
		glm::vec3 closestIntersect = glm::vec3(0, 0, 0);
		std::shared_ptr<Sphere> closestSphere = nullptr;

		// goes through game object list to check for collisions
		for (std::list<std::shared_ptr<Sphere> >::iterator it = sphereList.begin();
			it != sphereList.end();
			it++)
		{
			// if a game object collides finds where 
			if ((*it)->intersectTest(traced_ray))
			{
				//finds where it intersects
				glm::vec3 intersect = (*it)->intersectPos(traced_ray);
				rayLength = glm::length(intersect - traced_ray.origin);
				float distanceToSphere = glm::length((*it)->position - traced_ray.origin);

				// if it is the current shortest ray, isnt inside the sphere and infront of the camera set new closest values
				if (rayLength < shortestRay && rayLength < distanceToSphere)
				{
					shortestRay = rayLength;
					closestIntersect = intersect;
					closestSphere = (*it);
				}
			}

		}

		if (closestSphere != nullptr)
		{
			colour = (closestSphere->shade(traced_ray, closestIntersect));

			bool inShadow = shadowCheck(closestIntersect, closestSphere);
			if (inShadow)
			{
				colour = colour * glm::vec3(0.2, 0.2, 0.2);
			}
			

			if (closestSphere->isReflective == true)
			{
			
				//colour = (closestSphere->shade(traced_ray, closestIntersect)) * (1-closestSphere->reflectivity);
				float reflectionBias = 0.01f;

				glm::vec3 centerToIntersect = closestIntersect - closestSphere->position;
				glm::vec3 N = glm::normalize(centerToIntersect);
				//glm::vec3 A = traced_ray.dir - glm::dot(traced_ray.dir, N) * N;
;

				Ray reflectionRay;

				reflectionRay.origin = closestIntersect + (N * reflectionBias);

				//glm::vec3 I = traced_ray.origin - reflectionRay.origin;

				glm::vec3 R = glm::reflect(traced_ray.dir,N);
				reflectionRay.dir = R;

				//float rayOriginCheck = glm::length((closestSphere->position) - reflectionRay.origin);
				//if (rayOriginCheck > closestSphere->radius)
				{
					glm::vec3 reflectedColour = traceRay(reflectionRay) * closestSphere->reflectivity;// (1 - closestSphere->reflectivity);
					//colour = colour * (1 - closestSphere->reflectivity);// closestSphere->reflectivity; (1 - closestSphere->reflectivity)
					colour = colour * (1 - closestSphere->reflectivity) + reflectedColour;
					
				}
				
			}		
			
		}
		else
		{
			colour = glm::vec3(0.3, 0.4, 0.5);
		}

		return colour;

	}

	bool Tracer::shadowCheck(glm::vec3 intersect, std::shared_ptr<Sphere> sphere)
	{

		float shadowBias = 0.01f;
		glm::vec3 centerToIntersect = intersect - sphere->position;
		glm::vec3 N = glm::normalize(centerToIntersect);

		glm::vec3 lightDir = glm::normalize(lightPos - intersect);
		//glm::vec3 reflectDir = glm::reflect(-lightDir, N);

		Ray shadowRay;
		shadowRay.origin = intersect + (N * shadowBias);
		shadowRay.dir = lightDir;// glm::normalize(lightPos - intersect);

		// goes through game object list to check for collisions
		for (std::list<std::shared_ptr<Sphere> >::iterator it = sphereList.begin();
			it != sphereList.end();
			it++)
		{
			// if a game object collides finds where 
			if ((*it)->intersectTest(shadowRay))
			{
				//std::cout << "here" << std::endl;
				return true;
			}
		}
		return false;
	}

}

			//	//**********************************************************
			//	float shortestReflectedRay = 9999.9f;
			//	float ReflectedRayLength = 10000.0f;
			//	glm::vec3 closestReflectedIntersect = glm::vec3(0, 0, 0);
			//	std::shared_ptr<Sphere> closestReflectedSphere = nullptr;

			//	

			//	for (std::list<std::shared_ptr<Sphere> >::iterator it = sphereList.begin();
			//		it != sphereList.end();
			//		it++)
			//	{
			//		// if a game object collides finds where 
			//		if ((*it)->intersectTest(reflectionRay))
			//		{
			//			glm::vec3 reflectionIntersect = (*it)->intersectPos(reflectionRay);
			//			ReflectedRayLength = glm::length(reflectionIntersect - reflectionRay.origin);
			//			float distanceToSphere = glm::length((*it)->position - reflectionRay.origin);

			//			// if it is the current shortest ray, isnt inside the sphere and infront of the camera set new closest values
			//			if (rayLength < shortestRay && rayLength < distanceToSphere && distanceToSphere > 0)
			//			{
			//				shortestReflectedRay = ReflectedRayLength;
			//				closestReflectedIntersect = reflectionIntersect;
			//				closestReflectedSphere = (*it);
			//			}
			//		}
			//	}
			//	if (closestReflectedSphere != nullptr)
			//	{
			//		glm::vec3 reflectedSphereColour = (closestReflectedSphere->shade(reflectionRay, closestReflectedIntersect));
			//		glm::vec3 firstSphereColour = closestSphere->shade(traced_ray, closestIntersect);
			//		firstSphereColour = firstSphereColour * (1 - closestSphere->reflectivity);
			//		glm::vec3 combinedColour = firstSphereColour + reflectedSphereColour;
			//		std::cout << combinedColour.x << combinedColour.y << combinedColour.z << std::endl;
			//		return combinedColour;
			//	}
			//	else
			//	{

			//	}
			//} // end if (closestSphere->isReflective)

			//return (closestSphere->shade(traced_ray, closestIntersect));