#include "OpenGLVertextArray.h"
#include <glad/glad.h>
#include "OpenGLBuffer.h"

namespace gen
{
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_renderID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_renderID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_renderID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertextBuffer(const SP<OpenGLVertextBuffer>& vertexBuffer)
	{

		GEN_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
		glBindVertexArray(m_renderID);
		vertexBuffer->Bind();
		auto&& layout = vertexBuffer->GetLayout();
		for (auto&& element : layout)
		{
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(m_vertextBufferIndex);
				glVertexAttribPointer(m_vertextBufferIndex,
					element.ComponentCount,
					ShaderDataTypeUtils::ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				m_vertextBufferIndex++;
				break;			
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(m_vertextBufferIndex);
				glVertexAttribIPointer(m_vertextBufferIndex,
					element.ComponentCount,
					ShaderDataTypeUtils::ShaderDataTypeToOpenGLBaseType(element.Type),
					layout.GetStride(),
					(const void*)element.Offset
				);
				m_vertextBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.ComponentCount;
				for (uint8_t i = 0; i < count; ++i)
				{
					glEnableVertexAttribArray(m_vertextBufferIndex);
					glVertexAttribPointer(m_vertextBufferIndex,
						count,
						ShaderDataTypeUtils::ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset + sizeof(float) * count * i)
					);
					glVertexAttribDivisor(m_vertextBufferIndex, 1);
					m_vertextBufferIndex++;
				}
				break;
			}
			default:
				GEN_ASSERT(false, "Unkown ShaderDatType!");
			}
		}
		m_vertexBuffers.push_back(vertexBuffer);

	}

	void OpenGLVertexArray::SetIndexBuffer(const SP<OpenGLIndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_renderID);
		indexBuffer->Bind();
		m_indexBuufer = indexBuffer;
	}

	const std::vector<SP<OpenGLVertextBuffer>>& OpenGLVertexArray::GetVertexBuffers()
	{
		return m_vertexBuffers;
	}

	const SP<OpenGLIndexBuffer>& OpenGLVertexArray::GetIndexBuffer()
	{
		return m_indexBuufer;
	}

	SP<OpenGLVertexArray> OpenGLVertexArray::Create()
	{
		return std::make_shared<OpenGLVertexArray>();
	}
}


