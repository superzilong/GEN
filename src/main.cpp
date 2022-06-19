#include "Core/Log.h"
#include "OpenGL/OpenGLRenderAPI.h"
#include "Window/Window.h"

static void OnEvent(gen::Event& e)
{
	GEN_LOG_INFO(e.GetName());
}


int main(int argc, char* argv[])
{
	Log::Init();
	gen::WindowProps props;
	gen::Window win(props);
	win.SetEventCallback(OnEvent);
	gen::OpenGLRendererAPI::Init();
	while (true)
	{
		gen::OpenGLRendererAPI::SetClearColor({0.0, 1.0, 0.0, 1.0});
		gen::OpenGLRendererAPI::Clear();

		win.OnUpdate();
	}
	
	return 0;
}
