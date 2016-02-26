﻿// Copyright © 2016 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#include "Precompiled.h"

#include "Tonemappers/LinearTonemapper.h"
#include "Tracing/Scene.h"
#include "Rendering/Image.h"
#include "Rendering/Color.h"
#include "Math/MathUtils.h"

using namespace Raycer;

void LinearTonemapper::apply(const Scene& scene, const Image& inputImage, Image& outputImage)
{
	auto& inputPixelData = inputImage.getPixelDataConst();
	auto& outputPixelData = outputImage.getPixelData();

	const float invGamma = 1.0f / scene.tonemapping.gamma;
	
	#pragma omp parallel for
	for (int64_t i = 0; i < int64_t(inputPixelData.size()); ++i)
	{
		Color outputColor = inputPixelData.at(i);
		outputColor *= MathUtils::fastPow(2.0f, scene.tonemapping.exposure);

		if (scene.tonemapping.shouldClamp)
			outputColor.clamp();

		if (scene.tonemapping.applyGamma)
			outputColor = Color::fastPow(outputColor, invGamma);

		outputColor.a = 1.0f;
		outputPixelData[i] = outputColor;
	}
}
