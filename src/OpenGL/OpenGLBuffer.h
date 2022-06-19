#pragma once
#include <cstdint>
#include <string>
#include "Core/Assert.h"

typedef unsigned int GLenum;

namespace gen
{
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	class ShaderDataTypeUtils
	{
	public:
		static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type);
		static uint32_t ShaderDataTypeSize(ShaderDataType type);
		static uint32_t ShaderDataTypeComponentCount(ShaderDataType type);
	};

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t TypeSize;
		uint32_t ComponentCount;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), TypeSize(ShaderDataTypeUtils::ShaderDataTypeSize(type)),
			  ComponentCount(ShaderDataTypeUtils::ShaderDataTypeComponentCount(Type)), Offset(0), Normalized(normalized)
		{
		}
		
	};
	
	class BufferLayout
	{
	public:
		BufferLayout() = default;

		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_elements(elements)
		{
			CalculateOffsetAndStride();
		}

		uint32_t GetStride() const { return m_stride; }
		const std::vector<BufferElement>& GetElements() const { return m_elements; }

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
	private:
		void CalculateOffsetAndStride()
		{
			size_t offset = 0;
			m_stride = 0;
			for(auto&& elem : m_elements)
			{
				elem.Offset = offset;
				offset += elem.TypeSize;
				m_stride += elem.TypeSize;
			}
		}
		
		std::vector<BufferElement> m_elements;
		uint32_t m_stride = 0;
	};
	
	class OpenGLVertextBuffer
	{
	public:
		OpenGLVertextBuffer(uint32_t size);
		OpenGLVertextBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertextBuffer();

		void Bind() const;
		void Unbind() const;

		void SetData(const void* data, uint32_t size);

		const BufferLayout& GetLayout() const { return m_layout; }
	private:
		uint32_t m_renderID;
		BufferLayout m_layout;
	};

	class OpenGLIndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer();

		void Bind() const;
		void Unbind() const;
		uint32_t GetCount() const { return m_count; }
	private:
		uint32_t m_renderID;
		uint32_t m_count;
	};
	
}
