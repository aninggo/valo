﻿// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#include "Precompiled.h"

#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg/nanovg.h"
#include "nanovg/nanovg_gl.h"

#include "Rendering/InfoPanel.h"
#include "Rendering/Film.h"
#include "Runners/WindowRunner.h"
#include "Scenes/Scene.h"
#include "Utils/Settings.h"
#include "Utils/StringUtils.h"
#include "Tracers/TracerState.h"
#include "App.h"

using namespace Raycer;

InfoPanel::~InfoPanel()
{
	if (context != nullptr)
	{
		nvgDeleteGL3(context);
		context = nullptr;
	}
}

void InfoPanel::initialize()
{
	context = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

	if (context == nullptr)
		throw std::runtime_error("Could not initialize NanoVG");

	nvgCreateFont(context, "mono", "data/fonts/RobotoMono-Regular.ttf");
}

void InfoPanel::render(const TracerState& state)
{
	if (currentState == InfoPanelState::OFF)
		return;

	GLFWwindow* window = App::getWindowRunner().getGlfwWindow();
	int windowWidth, windowHeight, framebufferWidth, framebufferHeight;

	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

	float pixelRatio = float(framebufferWidth) / float(windowWidth);

	nvgBeginFrame(context, windowWidth, windowHeight, pixelRatio);

	if (currentState == InfoPanelState::FPS)
		renderFps();
	else if (currentState == InfoPanelState::FULL)
		renderFull(state);

	nvgEndFrame(context);
}

void InfoPanel::setState(InfoPanelState state)
{
	currentState = state;
}

void InfoPanel::selectNextState()
{
	if (currentState == InfoPanelState::OFF)
		currentState = InfoPanelState::FPS;
	else if (currentState == InfoPanelState::FPS)
		currentState = InfoPanelState::FULL;
	else if (currentState == InfoPanelState::FULL)
		currentState = InfoPanelState::OFF;
}

void InfoPanel::renderFps()
{
	Settings& settings = App::getSettings();
	const FpsCounter& fpsCounter = App::getWindowRunner().getFpsCounter();

	std::string fpsString = tfm::format("%.1f", fpsCounter.getFps());

	nvgBeginPath(context);

	nvgFontSize(context, float(settings.interactive.infoPanelFontSize));
	nvgFontFace(context, "mono");

	float lineSpacing;
	nvgTextMetrics(context, nullptr, nullptr, &lineSpacing);
	
	float bounds[4];
	nvgTextBounds(context, 0.0f, 0.0f, "1234567890.", nullptr, bounds);
	float charWidth = (bounds[2] - bounds[0]) / 11.0f;
	float panelWidth = fpsString.length() * charWidth;
	float panelHeight = bounds[3] - bounds[1];
	float currentX = charWidth / 2.0f + 2.0f;
	float currentY = -bounds[1] + 2.0f;

	float rounding = 5.0f;
	float strokeWidth = 2.0f;

	nvgFillColor(context, nvgRGBA(0, 0, 0, 150));
	nvgRoundedRect(context, -rounding, -rounding, panelWidth + rounding + charWidth, panelHeight + rounding, rounding);
	nvgFill(context);

	nvgBeginPath(context);

	nvgStrokeWidth(context, strokeWidth);
	nvgStrokeColor(context, nvgRGBA(0, 0, 0, 180));
	nvgRoundedRect(context, -rounding, -rounding, panelWidth + rounding + charWidth + strokeWidth / 2.0f, panelHeight + rounding + strokeWidth / 2.0f, rounding + 1.0f);
	nvgStroke(context);

	nvgFillColor(context, nvgRGBA(255, 255, 255, 255));
	nvgText(context, currentX, currentY, fpsString.c_str(), nullptr);
}

void InfoPanel::renderFull(const TracerState& state)
{
	Settings& settings = App::getSettings();
	WindowRunner& windowRunner = App::getWindowRunner();
	const FpsCounter& fpsCounter = windowRunner.getFpsCounter();
	const Scene& scene = *state.scene;
	const Film& film = *state.film;

	nvgBeginPath(context);

	nvgFontSize(context, float(settings.interactive.infoPanelFontSize));
	nvgFontFace(context, "mono");

	float lineSpacing;
	nvgTextMetrics(context, nullptr, nullptr, &lineSpacing);
	
	float bounds[4];
	nvgTextBounds(context, 0.0f, 0.0f, "1234567890.", nullptr, bounds);
	float charWidth = (bounds[2] - bounds[0]) / 11.0f;
	float panelWidth = 34 * charWidth;
	float panelHeight = 18 * lineSpacing + lineSpacing / 2.0f;
	float currentX = charWidth / 2.0f + 4.0f;
	float currentY = -bounds[1] + 4.0f;

	float rounding = 5.0f;
	float strokeWidth = 2.0f;

	nvgFillColor(context, nvgRGBA(0, 0, 0, 150));
	nvgRoundedRect(context, -rounding, -rounding, panelWidth + rounding + charWidth, panelHeight + rounding, rounding);
	nvgFill(context);

	nvgBeginPath(context);

	nvgStrokeWidth(context, strokeWidth);
	nvgStrokeColor(context, nvgRGBA(0, 0, 0, 180));
	nvgRoundedRect(context, -rounding, -rounding, panelWidth + rounding + charWidth + strokeWidth / 2.0f, panelHeight + rounding + strokeWidth / 2.0f, rounding + 1.0f);
	nvgStroke(context);

	nvgFillColor(context, nvgRGBA(255, 255, 255, 255));

	nvgText(context, currentX, currentY, tfm::format("FPS: %.1f", fpsCounter.getFps()).c_str(), nullptr);
	currentY += lineSpacing;

	nvgText(context, currentX, currentY, tfm::format("Frametime: %.1f ms", fpsCounter.getFrameTime()).c_str(), nullptr);
	currentY += lineSpacing;

	nvgText(context, currentX, currentY, tfm::format("Position: (%.2f, %.2f, %.2f)", scene.camera.position.x, scene.camera.position.y, scene.camera.position.z).c_str(), nullptr);
	currentY += lineSpacing;

	Vector3 direction = scene.camera.orientation.getDirection();
	nvgText(context, currentX, currentY, tfm::format("Direction: (%.2f, %.2f, %.2f)", direction.x, direction.y, direction.z).c_str(), nullptr);
	currentY += lineSpacing;

	nvgText(context, currentX, currentY, tfm::format("Rotation: (%.2f, %.2f, %.2f)", scene.camera.orientation.pitch, scene.camera.orientation.yaw, scene.camera.orientation.roll).c_str(), nullptr);
	currentY += lineSpacing;

	int64_t filmMouseX = std::max(int64_t(0), std::min(windowRunner.getMouseInfo().filmX, int64_t(film.getWidth() - 1)));
	int64_t filmMouseY = std::max(int64_t(0), std::min(windowRunner.getMouseInfo().filmY, int64_t(film.getHeight() - 1)));;
	int64_t filmMouseIndex = filmMouseY * film.getWidth() + filmMouseX;

	nvgText(context, currentX, currentY, tfm::format("Mouse: (%d, %d, %d)", filmMouseX, filmMouseY, filmMouseIndex).c_str(), nullptr);
	currentY += lineSpacing;

	Color linearColor = film.getLinearColor(filmMouseX, filmMouseY);
	Color outputColor = film.getOutputColor(filmMouseX, filmMouseY);

	nvgText(context, currentX, currentY, tfm::format("Pixel lin: (%.2f, %.2f, %.2f)", linearColor.r, linearColor.g, linearColor.b).c_str(), nullptr);
	currentY += lineSpacing;

	nvgText(context, currentX, currentY, tfm::format("Pixel out: (%.2f, %.2f, %.2f)", outputColor.r, outputColor.g, outputColor.b).c_str(), nullptr);
	currentY += lineSpacing;

	int tempWindowWidth, tempWindowHeight, tempFramebufferWidth, tempFramebufferHeight;
	GLFWwindow* window = windowRunner.getGlfwWindow();

	glfwGetWindowSize(window, &tempWindowWidth, &tempWindowHeight);
	glfwGetFramebufferSize(window, &tempFramebufferWidth, &tempFramebufferHeight);

	nvgText(context, currentX, currentY, tfm::format("Window: %dx%d (%dx%d)", tempWindowWidth, tempWindowHeight, tempFramebufferWidth, tempFramebufferHeight).c_str(), nullptr);
	currentY += lineSpacing;

	float totalPixels = float(film.getWidth() * film.getWidth());

	nvgText(context, currentX, currentY, tfm::format("Film: %dx%d (%.2fx) (%s)", film.getWidth(), film.getWidth(), settings.interactive.renderScale, StringUtils::humanizeNumber(totalPixels)).c_str(), nullptr);
	currentY += lineSpacing;

	float samplesPerSecond = float(state.sampleCount) * fpsCounter.getFps();

	nvgText(context, currentX, currentY, tfm::format("Samples/s: %s", StringUtils::humanizeNumber(samplesPerSecond)).c_str(), nullptr);
	currentY += lineSpacing;

	float pixelsPerSecond = float(state.pixelCount) * fpsCounter.getFps();

	nvgText(context, currentX, currentY, tfm::format("Pixels/s: %s", StringUtils::humanizeNumber(pixelsPerSecond)).c_str(), nullptr);
	currentY += lineSpacing;

	float raysPerSecond = float(state.rayCount) * fpsCounter.getFps();

	nvgText(context, currentX, currentY, tfm::format("Rays/s: %s", StringUtils::humanizeNumber(raysPerSecond)).c_str(), nullptr);
	currentY += lineSpacing;

	float pathsPerSecond = float(state.pathCount) * fpsCounter.getFps();

	nvgText(context, currentX, currentY, tfm::format("Paths/s: %s", StringUtils::humanizeNumber(pathsPerSecond)).c_str(), nullptr);
	currentY += lineSpacing;

	nvgText(context, currentX, currentY, tfm::format("Pixel samples: %d", film.getPixelSampleCount()).c_str(), nullptr);
	currentY += lineSpacing;

	nvgText(context, currentX, currentY, tfm::format("Moving: %s", scene.camera.isMoving()).c_str(), nullptr);
	currentY += lineSpacing;

	std::string tracerName = "unknown";

	switch (scene.general.tracerType)
	{
		case TracerType::RAY: tracerName = "ray"; break;
		case TracerType::PATH_RECURSIVE: tracerName = "path recursive"; break;
		case TracerType::PATH_ITERATIVE: tracerName = "path iterative"; break;
		case TracerType::PREVIEW: tracerName = "preview"; break;
		default: break;
	}

	nvgText(context, currentX, currentY, tfm::format("Tracer: %s", tracerName).c_str(), nullptr);
	currentY += lineSpacing;

	std::string tonemapperName = "unknown";

	switch (scene.tonemapping.type)
	{
		case TonemapperType::PASSTHROUGH: tonemapperName = "passthrough"; break;
		case TonemapperType::LINEAR: tonemapperName = "linear"; break;
		case TonemapperType::SIMPLE: tonemapperName = "simple"; break;
		case TonemapperType::REINHARD: tonemapperName = "reinhard"; break;
		default: break;
	}

	nvgText(context, currentX, currentY, tfm::format("Tonemapper: %s (%.2f/%.2f)", tonemapperName, scene.tonemapping.exposure, scene.tonemapping.key).c_str(), nullptr);
}