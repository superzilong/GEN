#include "Core/Log.h"
#include "OpenGL/OpenGLBuffer.h"
#include "OpenGL/OpenGLRenderAPI.h"
#include "OpenGL/OpenGLShader.h"
#include "OpenGL/OpenGLVertextArray.h"
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

	// Create vertext array object.
	gen::SP<gen::OpenGLVertexArray> vao = gen::OpenGLVertexArray::Create();

	// Create vertex buffer object.
	float vertices[6*7] = {
		-0.5f, -0.5f, 0.1f, 1.0f, 0.f, 0.f, 1.0f,
		0.0f, 0.5f, 0.1f, 0.f, 0.f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.1f, 0.f, 1.0f, 0.f, 1.0f,

		0.4f, -0.7f, 0.2f, 0.0f, 1.f, 1.f, 1.0f,
		0.0f, 0.7f, 0.2f, 0.f, 1.f, 1.0f, 1.0f,
		0.8f, -0.5f, 0.2f, 0.f, 1.0f, 1.f, 1.0f
	};
	gen::SP<gen::OpenGLVertextBuffer> vbo = gen::OpenGLVertextBuffer::create(vertices, sizeof(vertices));
	gen::BufferLayout layout = {
		{ gen::ShaderDataType::Float3, "a_Position"},
		{gen::ShaderDataType::Float4, "a_Color"}
	};
	vbo->SetLayout(layout);
	vao->AddVertextBuffer(vbo);

	// Create element buffer object.
	uint32_t indices[6] = { 0, 1, 2, 3, 4, 5 };
	gen::SP<gen::OpenGLElementBuffer> ebo = gen::OpenGLElementBuffer::create(indices, sizeof(indices) / sizeof(uint32_t));
	vao->SetElementBuffer(ebo);

	std::string vertexShaderSrc = R"(
		#version 330 core
		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;

		out vec3 v_Position;
		out vec4 v_Color;

		void main()
		{
			gl_Position = vec4(a_Position, 1.0);
			v_Position = a_Position;
			v_Color = a_Color;
		}
	)";

	std::string fragmentShaderSrc = R"(
		#version 330 core

		layout(location = 0) out vec4 color;

		in vec3 v_Position;
		in vec4 v_Color;

		void main()
		{
            //color = vec4(v_Position * 0.5 + 0.5, 1.0);
			color = v_Color;
		}
	)";

	gen::SP<gen::OpenGLShader> shader = gen::OpenGLShader::Create("VertexColor", vertexShaderSrc, fragmentShaderSrc);

	
	while (true)
	{
		gen::OpenGLRendererAPI::SetClearColor({0.8, 0.8, 0.8, 1.0});
		gen::OpenGLRendererAPI::Clear();

		shader->Bind();
		vao->Bind();
		gen::OpenGLRendererAPI::DrawIndexed(vao);
		win.OnUpdate();
	}
	
	return 0;
}
