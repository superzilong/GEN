#pragma once
#include <glm/vec4.hpp>
#include "Core/Base.h"

namespace gen
{
	class OpenGLVertexArray;

	class OpenGLRendererAPI
	{
	public:
		static void Init();
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();

		static void DrawIndexed(const SP<OpenGLVertexArray>& vertexArray, uint32_t indexCount = 0);
	};
	
}
