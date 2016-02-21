// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#include "Precompiled.h"

#include "TestScenes/TestScene.h"
#include "Tracing/Scene.h"
#include "App.h"
#include "Utils/Log.h"

using namespace Raycer;

Scene TestScene::create(uint64_t number)
{
	App::getLog().logInfo("Creating test scene number %d", number);

	switch (number)
	{
		case 1: return create1();
		case 2: return create2();
		case 3: return create3();
		default: throw std::runtime_error("Unknown test scene number");
	}
}
