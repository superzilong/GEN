#pragma once
#include "Core/Base.h"

namespace gen
{
	class OpenGLIndexBuffer;
	class OpenGLVertextBuffer;

	class OpenGLVertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertextBuffer(const SP<OpenGLVertextBuffer>& vertexBuffer);
		void SetIndexBuffer(const SP<OpenGLIndexBuffer>& indexBuffer);

		const std::vector<SP<OpenGLVertextBuffer>>& GetVertexBuffers();
		const SP<OpenGLIndexBuffer>& GetIndexBuffer();

		static SP<OpenGLVertexArray> Create();
	private:
		uint32_t m_renderID = 0;
		uint32_t m_vertextBufferIndex = 0;
		std::vector<SP<OpenGLVertextBuffer>> m_vertexBuffers;
		SP<OpenGLIndexBuffer> m_indexBuufer;
	};
}
