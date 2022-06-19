#pragma once

struct GLFWwindow;

namespace gen {

	class OpenGLRenderContext
	{
	public:
		OpenGLRenderContext(GLFWwindow* windowHandle);

		virtual void Init();
		virtual void SwapBuffers();
	private:
		GLFWwindow* m_windowHandle;
	};

}