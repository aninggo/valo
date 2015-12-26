﻿// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#pragma once

#include <atomic>
#include <memory>

#include "RunnerStates/RunnerState.h"
#include "Tracers/Tracer.h"
#include "Scenes/Scene.h"
#include "Rendering/Film.h"
#include "Rendering/FilmRenderer.h"
#include "Rendering/InfoPanel.h"

namespace Raycer
{
	class DefaultState : public RunnerState
	{
	public:

		DefaultState();

		void initialize() override;
		void pause() override;
		void resume() override;
		void shutdown() override;

		void update(double timeStep) override;
		void render(double timeStep, double interpolation) override;

		void windowResized(uint64_t width, uint64_t height) override;

	private:

		void resizeFilm();

		Scene scene;
		Film film;
		FilmRenderer filmRenderer;
		InfoPanel infoPanel;

		std::unique_ptr<Tracer> tracer;

		std::atomic<bool> interrupted;
		uint64_t currentTestSceneNumber = 1;
	};
}
