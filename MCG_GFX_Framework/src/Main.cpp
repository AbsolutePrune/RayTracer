
#include <cmath>
#include <mutex>
#include "Sphere.h"
#include <Thread>
#include <iostream>
#include <chrono>
#include "MCG_GFX_Lib.h"
#include "Camera.h"
#include "Tracer.h"


	int main(int argc, char* argv[])
	{
		// Variable for storing window dimensions
		glm::ivec2 windowSize(640, 480);
		//glm::ivec2 windowSize = MCG::Thread::getWindowSize();

		std::mutex drawMutex;

		// Call MCG::Init to initialise and create your window
		// Tell it what size you want the window to be
		if (!MCG::Init(windowSize))
		{
			// We must check if something went wrong
			// (this is very unlikely)
			return -1;
		}

		std::shared_ptr<MCG::Camera> camera = std::make_shared<MCG::Camera>();
		std::shared_ptr<MCG::Tracer> tracer = std::make_shared<MCG::Tracer>();
		

		// Sets every pixel to the same colour
		// parameters are RGB, numbers are from 0 to 1
		MCG::SetBackground(glm::vec3(0.1, 0.1, 0.1));

		std::cout << "how many threads? " << std::endl;
		int threadCount;
		std::cin >> threadCount;
		std::cout << threadCount << " threads" << std::endl;
		int collumnWidth = windowSize.x / threadCount;
		std::cout << collumnWidth << " collumnWidth" << std::endl;
		std::list<std::thread> threadList;

		std::chrono::steady_clock::time_point time1 = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < threadCount; i++)
		{
			threadList.push_back(std::thread([tracer, camera, windowSize, i, collumnWidth, &drawMutex]()
				{
					// Preparing a position to draw a pixel
					glm::ivec2 pixelPosition = glm::ivec2(0 + (collumnWidth * i), 0);

					//loop for all pixels
					while (pixelPosition.x < (collumnWidth * (i + 1)))
					{
						while (pixelPosition.y < windowSize.y)
						{
							MCG::Ray ray = camera->fireRay(pixelPosition);

							glm::vec3 colour = tracer->traceRay(ray);

							drawMutex.lock();
							// Draws a single pixel at the specified coordinates in the specified colour!
							MCG::DrawPixel(pixelPosition, colour);
							drawMutex.unlock();
							pixelPosition.y++;
						}
						// Draws a single pixel at the specified coordinates in the specified colour!
						pixelPosition.y = 0;
						pixelPosition.x++;

					}
				}
			));
		}


		std::list<std::thread>::iterator it;
		for (it = threadList.begin(); it != threadList.end(); ++it)
		{			
			it->join();
		}


		std::chrono::steady_clock::time_point time2 = std::chrono::high_resolution_clock::now();
		std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1);
		std::cout << "Time taken: " << milliseconds.count() << std::endl;
		std::cout << "Sphere Count: " << tracer->length << std::endl;


		return MCG::ShowAndHold();
	}

	