#include "OpenGLRenderContext.h"

#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Core/Base.h"

namespace gen
{
	OpenGLRenderContext::OpenGLRenderContext(GLFWwindow* windowHandle) : m_windowHandle(windowHandle)
	{
		GEN_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLRenderContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		GEN_ASSERT(status, "Failed to initialize Glad!");

		GEN_LOG_INFO("OpenGL Info:");
		GEN_LOG_INFO("  Vendor:  {0}", glGetString(GL_VENDOR));
		GEN_LOG_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		GEN_LOG_INFO("  Version: {0}", glGetString(GL_VERSION));

		GEN_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Hazel requires at least OpenGL version 4.5!");
	}

	void OpenGLRenderContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}
}
