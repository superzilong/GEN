#include "OpenGLRenderAPI.h"

#include <glad/glad.h>

#include "OpenGLBuffer.h"
#include "OpenGLVertextArray.h"

namespace gen
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         GEN_LOG_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       GEN_LOG_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          GEN_LOG_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: GEN_LOG_TRACE(message); return;
		}
		GEN_ASSERT(false, "Unknown severity level!");
	}
	
	void OpenGLRendererAPI::Init()
	{
#ifndef NDEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
#endif
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);	
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const SP<OpenGLVertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? indexCount : vertexArray->GetElementBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
