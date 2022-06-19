#pragma once

#include <functional>
#include <GLFW/glfw3.h>
#include "Events/Event.h"
#include <string>

namespace gen {
	class OpenGLRenderContext;
}

namespace gen {

	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Graphic Engine",
			uint32_t width = 1600,
			uint32_t height = 900)
			: Title(title), Width(width), Height(height)
		{
		}
	};
	
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		
		Window(const WindowProps& props);
		virtual ~Window();

		void OnUpdate();

		uint32_t GetWidth() const { return m_data.Width; }
		uint32_t GetHeight() const{ return m_data.Height; }

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) { m_data.EventCallback = callback; }
		void SetVSync(bool enabled);
		bool IsVSync() const;

		virtual void* GetNativeWindow() const { return m_glfwWin; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		GLFWwindow* m_glfwWin = nullptr;
		std::unique_ptr<OpenGLRenderContext> m_context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_data;
	};
}
