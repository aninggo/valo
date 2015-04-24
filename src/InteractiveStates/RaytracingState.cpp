// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#include "InteractiveStates/RaytracingState.h"
#include "App.h"
#include "Utils/Log.h"
#include "Utils/Settings.h"
#include "Runners/InteractiveRunner.h"
#include "Rendering/Framebuffer.h"
#include "Raytracing/Raytracer.h"

using namespace Raycer;

void RaytracingState::initialize()
{
	//Settings& settings = App::getSettings();
	Framebuffer& framebuffer = App::getFramebuffer();

	//scene.loadFromFile(settings.scene.fileName);
	scene = Scene::createTestScene();
	scene.initialize();
	scene.camera.setImagePlaneSize(framebuffer.getWidth(), framebuffer.getHeight());
}

void RaytracingState::pause()
{
}

void RaytracingState::resume()
{
}

void RaytracingState::shutdown()
{
}

void RaytracingState::update(double timeStep)
{
	scene.camera.update(timeStep);
	scene.camera.precalculate();
}

void RaytracingState::render(double timeStep, double interpolation)
{
	(void)timeStep;
	(void)interpolation;

	Framebuffer& framebuffer = App::getFramebuffer();
	Settings& settings = App::getSettings();
	Raytracer& raytracer = App::getRaytracer();
	InteractiveRunner& runner = App::getInteractiveRunner();
	Text& text = runner.getDefaultText();

	state.renderTarget = &framebuffer;
	state.scene = &scene;
	state.sceneWidth = framebuffer.getWidth();
	state.sceneHeight = framebuffer.getHeight();
	state.pixelOffset = 0;
	state.pixelCount = state.sceneWidth * state.sceneHeight;
	state.pixelsProcessed = 0;
	state.raysProcessed = 0;
	state.isInteractive = true;

	raytracer.run(state, interrupted);

	if (settings.window.showCameraInfo)
	{
		text.drawText(5.0, (double)(runner.getWindowHeight() - 3 * settings.window.defaultFontSize), Color(255, 255, 255, 255), tfm::format("Pos: (%.2f, %.2f, %.2f)", scene.camera.position.x, scene.camera.position.y, scene.camera.position.z));
		text.drawText(5.0, (double)(runner.getWindowHeight() - 4 * settings.window.defaultFontSize - 2), Color(255, 255, 255, 255), tfm::format("Rot: (%.2f, %.2f, %.2f)", scene.camera.orientation.yaw, scene.camera.orientation.pitch, scene.camera.orientation.roll));
		text.drawText(5.0, (double)(runner.getWindowHeight() - 5 * settings.window.defaultFontSize - 4), Color(255, 255, 255, 255), tfm::format("Rays: %d", state.raysProcessed.load()));
	}
}

void RaytracingState::windowResized(int width, int height)
{
	(void)width;
	(void)height;
}

void RaytracingState::framebufferResized(int width, int height)
{
	scene.camera.setImagePlaneSize(width, height);
}
