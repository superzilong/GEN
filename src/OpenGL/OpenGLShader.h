#pragma once
#include <memory>
#include <string>
#include <glm/glm.hpp>

#include "Core/Base.h"
#include <glm/glm.hpp>

// TODO: REMOVE!
typedef unsigned int GLenum;

namespace gen
{
	class OpenGLShader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		void Bind() const;
		void Unbind() const;

		void SetInt(const std::string& name, int value);
		void SetIntArray(const std::string& name, int* values, uint32_t count);
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, const glm::vec2& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetMat4(const std::string& name, const glm::mat4& value);

		const std::string& GetName() const { return m_name; }

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		static SP<OpenGLShader> Create(const std::string& filepath);
		static SP<OpenGLShader> Create(const std::string& name, const std::string& veterSrc, const std::string& fragmentSrc );
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

		uint32_t m_rendererID = 0;
		std::string m_name;
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::string& name, const SP<OpenGLShader>& shader);
		void Add(SP<OpenGLShader>& shader);
		SP<OpenGLShader> Load(const std::string& filepath);
		SP<OpenGLShader> Load(const std::string& name, const std::string& filepath);

		SP<OpenGLShader> Get(const std::string& name);
		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, SP<OpenGLShader>> m_shaders;
	};
}
