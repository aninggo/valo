// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#pragma once

#include <cstdint>

#include <GL/glew.h>

#include "Rendering/RenderTarget.h"

namespace Raycer
{
	class Color;

	class Framebuffer : public RenderTarget
	{
	public:

		Framebuffer();
		~Framebuffer();

		void initialize();
		void setSize(int width, int height);
		void setPixel(int x, int y, const Color& color);
		void setPixel(int index, const Color& color);
		Color getPixel(int x, int y) const;

		uint32_t* getPixelData() const;
		uint32_t getCpuTextureId() const;
		uint32_t getGpuTextureId() const;
		int getWidth() const;
		int getHeight() const;

		void clear();
		void clear(const Color& color);
		void render() const;

		void enableSmoothing(bool state);

	private:

		Framebuffer(const Framebuffer& framebuffer);
		Framebuffer& operator=(const Framebuffer& framebuffer);

		int width = 0;
		int height = 0;

		uint32_t* pixelData = nullptr;

		GLuint cpuTextureId = 0;
		GLuint gpuTextureId = 0;
		GLuint programId = 0;
		GLuint vaoId = 0;
		GLuint vertexBufferId = 0;
		GLuint samplerUniformId = 0;
		GLuint textureWidthUniformId = 0;
		GLuint textureHeightUniformId = 0;
		GLuint texelWidthUniformId = 0;
		GLuint texelHeightUniformId = 0;
	};
}
