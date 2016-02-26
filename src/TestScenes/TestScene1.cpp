﻿// Copyright © 2016 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#include "Precompiled.h"

#include "TestScenes/TestScene.h"
#include "Tracing/Scene.h"

using namespace Raycer;

// CORNELL BOX CUBOIDS //

Scene TestScene::create1()
{
	Scene scene;

	scene.general.tracerType = TracerType::RAY;

	scene.pathtracing.enableMultiSampling = true;
	scene.pathtracing.multiSamplerFilterType = FilterType::BELL;
	scene.pathtracing.minPathLength = 3;
	scene.pathtracing.terminationProbability = 0.2f;
	scene.pathtracing.pixelSampleCount = 1;

	scene.camera.position = Vector3(0.0f, 1.0f, 3.5f);

	scene.bvhInfo.bvhType = BVHType::BVH4;
	scene.bvhInfo.maxLeafSize = 4;

	ModelLoaderInfo model;
	model.modelFilePath = "data/models/cornellbox-cuboids/cornellbox.obj";

	scene.models.push_back(model);
	
	AmbientLight ambientLight;
	ambientLight.color = Color(1.0f, 1.0f, 1.0f) * 0.01f;
	ambientLight.occlusion = false;
	ambientLight.maxSampleDistance = 0.2f;
	ambientLight.sampleCountSqrt = 4;

	scene.lights.ambientLights.push_back(ambientLight);

	PointLight pointLight;
	pointLight.color = Color(1.0f, 0.71f, 0.24f) * 1.0f;
	pointLight.position = Vector3(0.0f, 1.7f, 0.0f);

	scene.lights.pointLights.push_back(pointLight);

	AreaPointLight areaLight;
	areaLight.color = Color(1.0f, 0.71f, 0.24f) * 1.0f;
	areaLight.position = Vector3(0.0f, 1.7f, 0.0f);
	areaLight.radius = 0.2f;
	areaLight.sampleCountSqrt = 4;

	//scene.lights.areaPointLights.push_back(areaLight);

	return scene;
}
