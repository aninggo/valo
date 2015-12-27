﻿// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#include "Precompiled.h"

#include "Runners/ConsoleRunner.h"
#include "App.h"
#include "Utils/Settings.h"
#include "Utils/StringUtils.h"
#include "Utils/SysUtils.h"
#include "Rendering/Film.h"
#include "Scenes/Scene.h"
#include "Tracers/Tracer.h"
#include "Tracers/TracerState.h"
#include "Tracing/Camera.h"

using namespace Raycer;
using namespace std::chrono;

int ConsoleRunner::run()
{
	Settings& settings = App::getSettings();

	interrupted = false;

	pixelsPerSecondAverage.setAlpha(0.05);
	pixelsPerSecondAverage.setAverage(0.0);
	timer.setAveragingAlpha(0.05);

	Scene scene;

	if (settings.scene.enableTestScenes)
		scene = Scene::createTestScene(settings.scene.testSceneNumber);
	else
		scene = Scene::loadFromFile(settings.scene.fileName);

	scene.initialize();
	scene.camera.setImagePlaneSize(settings.image.width, settings.image.height);
	scene.camera.update(0.0);

	Film film;
	film.resize(settings.image.width, settings.image.height);

	if (settings.film.restoreFromFile)
		film.load(settings.film.restoreFileName);

	TracerState state;
	state.scene = &scene;
	state.film = &film;
	state.filmWidth = settings.image.width;
	state.filmHeight = settings.image.height;
	state.pixelStartOffset = 0;
	state.pixelCount = state.filmWidth * state.filmHeight;

	run(state);

	if (!interrupted)
	{
		film.getOutputImage().save(settings.image.fileName);

		if (settings.image.autoView)
			SysUtils::openFileExternally(settings.image.fileName);
	}
	else
		film.getOutputImage().save("partial_result.png");

	return 0;
}

void ConsoleRunner::run(TracerState& state)
{
	Settings& settings = App::getSettings();
	
	interrupted = false;

	std::atomic<bool> renderThreadFinished(false);
	std::exception_ptr renderThreadException = nullptr;

	auto renderThreadFunction = [&]()
	{
		try
		{
			Tracer::getTracer(state.scene->general.tracerType)->run(state, interrupted);
		}
		catch (...)
		{
			renderThreadException = std::current_exception();
		}

		renderThreadFinished = true;
	};

	SysUtils::setConsoleTextColor(ConsoleTextColor::WHITE_ON_BLACK);

	std::cout << tfm::format("\nTracing started (dimensions: %dx%d, offset: %d, pixels: %d, size: %fB, threads: %d)\n\n",
		state.filmWidth,
		state.filmHeight,
		state.pixelStartOffset,
		state.pixelCount,
		StringUtils::humanizeNumber(double(state.pixelCount * sizeof(Color)), true),
		settings.general.maxThreadCount);

	timer.setTargetValue(double(state.pixelCount));
	timer.restart();

	std::thread renderThread(renderThreadFunction);

	while (!renderThreadFinished)
	{
		timer.updateCurrentValue(double(state.pixelsProcessed));

		auto elapsed = timer.getElapsed();
		auto remaining = timer.getRemaining();

		if (elapsed.totalMilliseconds > 0)
			pixelsPerSecondAverage.addMeasurement(double(state.pixelsProcessed) / (double(elapsed.totalMilliseconds) / 1000.0));

		printProgress(elapsed, remaining);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	renderThread.join();

	if (renderThreadException != nullptr)
		std::rethrow_exception(renderThreadException);

	timer.updateCurrentValue(double(state.pixelsProcessed));

	auto elapsed = timer.getElapsed();
	auto remaining = timer.getRemaining();

	printProgress(elapsed, remaining);

	double totalPixelsPerSecond = 0.0;

	if (elapsed.totalMilliseconds > 0)
		totalPixelsPerSecond = double(state.pixelsProcessed) / (double(elapsed.totalMilliseconds) / 1000.0);

	std::cout << tfm::format("\n\nTracing %s (time: %s, pixels/s: %f)\n\n",
		interrupted ? "interrupted" : "finished",
		elapsed.getString(true),
		StringUtils::humanizeNumber(totalPixelsPerSecond));

	SysUtils::setConsoleTextColor(ConsoleTextColor::DEFAULT);

	state.film->generateOutputImage(*state.scene);
}

void ConsoleRunner::interrupt()
{
	interrupted = true;
}

void ConsoleRunner::printProgress(const TimerData& elapsed, const TimerData& remaining)
{
	uint64_t percentage = uint64_t(timer.getPercentage() + 0.5);
	uint64_t barCount = percentage / 4;

    tfm::printf("[");

	for (uint64_t i = 0; i < barCount; ++i)
        tfm::printf("=");

	if (barCount < 25)
	{
        tfm::printf(">");

		for (uint64_t i = 0; i < (24 - barCount); ++i)
            tfm::printf(" ");
	}

    tfm::printf("] ");
    tfm::printf("%d %% | ", percentage);
    tfm::printf("Elapsed time: %s | ", elapsed.getString());
    tfm::printf("Remaining time: %s | ", remaining.getString());
    tfm::printf("Pixels/s: %s", StringUtils::humanizeNumber(pixelsPerSecondAverage.getAverage()));
    tfm::printf("          \r");
}
