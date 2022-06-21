#include "OpenGLBuffer.h"
#include <glad/glad.h>

namespace gen
{
	GLenum ShaderDataTypeUtils::ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}
		GEN_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	uint32_t ShaderDataTypeUtils::ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return 4;
		case ShaderDataType::Float2: return 4 * 2;
		case ShaderDataType::Float3: return 4 * 3;
		case ShaderDataType::Float4: return 4 * 4;
		case ShaderDataType::Mat3: return 4 * 3 * 3;
		case ShaderDataType::Mat4: return 4 * 4 * 4;
		case ShaderDataType::Int: return 4;
		case ShaderDataType::Int2: return 4 * 2;
		case ShaderDataType::Int3: return 4 * 3;
		case ShaderDataType::Int4: return 4 * 4;
		case ShaderDataType::Bool: return 1;
		default:
			GEN_ASSERT(false, "Unsupported ShaderDataType!");
			return 0;
		}
	}

	uint32_t ShaderDataTypeUtils::ShaderDataTypeComponentCount(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return 1;
		case ShaderDataType::Float2: return 2;
		case ShaderDataType::Float3: return 3;
		case ShaderDataType::Float4: return 4;
		case ShaderDataType::Mat3: return 3; // 3*float3
		case ShaderDataType::Mat4: return 4; // 4*float4
		case ShaderDataType::Int: return 1;
		case ShaderDataType::Int2: return 2;
		case ShaderDataType::Int3: return 3;
		case ShaderDataType::Int4: return 4;
		case ShaderDataType::Bool: return 1;
		default:
			GEN_ASSERT(false, "Unsupported ShaderDataType!");
			return 0;
		}
	}

	SP<OpenGLVertextBuffer> OpenGLVertextBuffer::create(uint32_t size)
	{
		return std::make_shared<OpenGLVertextBuffer>(size);
	}

	SP<OpenGLVertextBuffer> OpenGLVertextBuffer::create(float* vertices, uint32_t size)
	{
		return std::make_shared<OpenGLVertextBuffer>(vertices, size);
	}

	OpenGLVertextBuffer::OpenGLVertextBuffer(uint32_t size)
	{
		glCreateBuffers(1, &m_renderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertextBuffer::OpenGLVertextBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_renderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
	}

	OpenGLVertextBuffer::~OpenGLVertextBuffer()
	{
		glDeleteBuffers(1, &m_renderID);
	}

	void OpenGLVertextBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
	}

	void OpenGLVertextBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertextBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	SP<OpenGLElementBuffer> OpenGLElementBuffer::create(uint32_t* indices, uint32_t count)
	{
		return std::make_shared<OpenGLElementBuffer>(indices, count);
	}

	OpenGLElementBuffer::OpenGLElementBuffer(uint32_t* indices, uint32_t count) : m_count(count)
	{
		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		glCreateBuffers(1, &m_renderID);
		glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLElementBuffer::~OpenGLElementBuffer()
	{
		glDeleteBuffers(1, &m_renderID);
	}

	void OpenGLElementBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID);
	}

	void OpenGLElementBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

