#include <iostream>
#include <glad/glad.h>

#include "Core/Log.h"
#include "OpenGL/OpenGLBuffer.h"
#include "OpenGL/OpenGLRenderAPI.h"
#include "OpenGL/OpenGLShader.h"
#include "OpenGL/OpenGLVertextArray.h"
#include "OpenGL/OpenGLCamera.h"
#include "Window/Window.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

static void OnEvent(gen::Event& e)
{
	GEN_LOG_INFO(e.GetName());
}

std::pair<gen::SP<gen::OpenGLVertexArray>, gen::SP<gen::OpenGLShader>> test()
{
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
	return { vao, shader };
};

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
};

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(int argc, char* argv[])
{
	Log::Init();
	gen::WindowProps props;
	gen::Window win(props);
	win.SetEventCallback(OnEvent);
	gen::OpenGLRendererAPI::Init();

	auto lightShader = gen::OpenGLShader::Create(".\\Cases\\Multilights\\light_cube.vs", ".\\Cases\\Multilights\\light_cube.fs");
	auto bodyShader = gen::OpenGLShader::Create(".\\Cases\\Multilights\\multiple_lights.vs", ".\\Cases\\Multilights\\multiple_lights.fs");

	gen::OpenGLCamera camera(glm::vec3(0.0f, 0.0f, 3.0f));
	// set up vertex data (and buffer(s)) and configure vertex attributes
 // ------------------------------------------------------------------
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	// positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	// first, configure the cube's VAO (and VBO)
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	unsigned int diffuseMap = loadTexture("./Cases/Multilights/resources/textures/container2.png");
	unsigned int specularMap = loadTexture("/Cases/Multilights/resources/textures/container2_specular.png");

	// shader configuration
	// --------------------
	bodyShader->Bind();
	bodyShader->SetInt("material.diffuse", 0);
	bodyShader->SetInt("material.specular", 1);
	
	while (true)
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		gen::OpenGLRendererAPI::SetClearColor({0.1, 0.1, 0.1, 1.0});
		gen::OpenGLRendererAPI::Clear();


		// be sure to activate shader when setting uniforms/drawing objects
		bodyShader->Bind();
		bodyShader->SetFloat3("viewPos", camera.Position);
		bodyShader->SetFloat("material.shininess", 32.0f);

		/*
		   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
		*/
		// directional light
		bodyShader->SetFloat3("dirLight.direction", { -0.2f, -1.0f, -0.3f });
		bodyShader->SetFloat3("dirLight.ambient", { 0.05f, 0.05f, 0.05f });
		bodyShader->SetFloat3("dirLight.diffuse", {0.4f, 0.4f, 0.4f});
		bodyShader->SetFloat3("dirLight.specular", {0.5f, 0.5f, 0.5f});
		// point light 1
		bodyShader->SetFloat3("pointLights[0].position", pointLightPositions[0]);
		bodyShader->SetFloat3("pointLights[0].ambient", { 0.05f, 0.05f, 0.05f });
		bodyShader->SetFloat3("pointLights[0].diffuse", { 0.8f, 0.8f, 0.8f });
		bodyShader->SetFloat3("pointLights[0].specular", {1.0f, 1.0f, 1.0f});
		bodyShader->SetFloat("pointLights[0].constant", 1.0f);
		bodyShader->SetFloat("pointLights[0].linear", 0.09f);
		bodyShader->SetFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		bodyShader->SetFloat3("pointLights[1].position", pointLightPositions[1]);
		bodyShader->SetFloat3("pointLights[1].ambient", {0.05f, 0.05f, 0.05f});
		bodyShader->SetFloat3("pointLights[1].diffuse", {0.8f, 0.8f, 0.8f});
		bodyShader->SetFloat3("pointLights[1].specular", {1.0f, 1.0f, 1.0f});
		bodyShader->SetFloat("pointLights[1].constant", 1.0f);
		bodyShader->SetFloat("pointLights[1].linear", 0.09f);
		bodyShader->SetFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		bodyShader->SetFloat3("pointLights[2].position", pointLightPositions[2]);
		bodyShader->SetFloat3("pointLights[2].ambient", { 0.05f, 0.05f, 0.05f });
		bodyShader->SetFloat3("pointLights[2].diffuse", {0.8f, 0.8f, 0.8f});
		bodyShader->SetFloat3("pointLights[2].specular", {1.0f, 1.0f, 1.0f});
		bodyShader->SetFloat("pointLights[2].constant", 1.0f);
		bodyShader->SetFloat("pointLights[2].linear", 0.09f);
		bodyShader->SetFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		bodyShader->SetFloat3("pointLights[3].position", pointLightPositions[3]);
		bodyShader->SetFloat3("pointLights[3].ambient", {0.05f, 0.05f, 0.05f});
		bodyShader->SetFloat3("pointLights[3].diffuse", {0.8f, 0.8f, 0.8f});
		bodyShader->SetFloat3("pointLights[3].specular", {1.0f, 1.0f, 1.0f});
		bodyShader->SetFloat("pointLights[3].constant", 1.0f);
		bodyShader->SetFloat("pointLights[3].linear", 0.09f);
		bodyShader->SetFloat("pointLights[3].quadratic", 0.032f);
		// spotLight
		bodyShader->SetFloat3("spotLight.position", camera.Position);
		bodyShader->SetFloat3("spotLight.direction", camera.Front);
		bodyShader->SetFloat3("spotLight.ambient", {0.0f, 0.0f, 0.0f});
		bodyShader->SetFloat3("spotLight.diffuse", {1.0f, 1.0f, 1.0f});
		bodyShader->SetFloat3("spotLight.specular", {1.0f, 1.0f, 1.0f});
		bodyShader->SetFloat("spotLight.constant", 1.0f);
		bodyShader->SetFloat("spotLight.linear", 0.09f);
		bodyShader->SetFloat("spotLight.quadratic", 0.032f);
		bodyShader->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		bodyShader->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)props.Width / (float)props.Height, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		bodyShader->SetMat4("projection", projection);
		bodyShader->SetMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		bodyShader->SetMat4("model", model);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		// render containers
		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			bodyShader->SetMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// also draw the lamp object(s)
		lightShader->Bind();
		lightShader->SetMat4("projection", projection);
		lightShader->SetMat4("view", view);

		// we now draw as many light bulbs as we have point lights.
		glBindVertexArray(lightCubeVAO);
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lightShader->SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		//gen::OpenGLRendererAPI::DrawIndexed(vao);
		win.OnUpdate();
	}
	
	return 0;
}
