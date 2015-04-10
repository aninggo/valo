// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#include "glfw/glfw3.h"

#include "States/GpuTracingState.h"
#include "App.h"
#include "Utils/Log.h"
#include "Utils/Settings.h"
#include "Runners/InteractiveRunner.h"
#include "Rendering/Framebuffer.h"
#include "GpuRaytracing/GpuRaytracer.h"

using namespace Raycer;

void GpuTracingState::initialize()
{
	Framebuffer& framebuffer = App::getFramebuffer();
	GpuRaytracer& gpuRaytracer = App::getGpuRaytracer();

	gpuRaytracer.initialize();

	scene.load("data/scenes/test_scene.json");
	scene.initialize();
	scene.camera.setImagePlaneSize(framebuffer.getWidth(), framebuffer.getHeight());
	scene.camera.calculateVariables();
}

void GpuTracingState::pause()
{

}

void GpuTracingState::resume()
{

}

void GpuTracingState::shutdown()
{

}

void GpuTracingState::update(double timeStep)
{
	scene.camera.update(timeStep);
}

void GpuTracingState::render(double timeStep, double interpolation)
{
	(void)timeStep;
	(void)interpolation;

	GpuRaytracer& gpuRaytracer = App::getGpuRaytracer();

	pixelCount = 0;
	rayCount = 0;

	gpuRaytracer.trace(scene, interrupted, pixelCount, rayCount);
}

void GpuTracingState::windowResized(int width, int height)
{
	(void)width;
	(void)height;
}

void GpuTracingState::framebufferResized(int width, int height)
{
	scene.camera.setImagePlaneSize(width, height);
}
