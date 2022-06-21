#pragma once
#include "Core/Base.h"

namespace gen
{
	class OpenGLElementBuffer;
	class OpenGLVertextBuffer;

	class OpenGLVertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertextBuffer(const SP<OpenGLVertextBuffer>& vertexBuffer);
		void SetElementBuffer(const SP<OpenGLElementBuffer>& elementBuffer);

		const std::vector<SP<OpenGLVertextBuffer>>& GetVertexBuffers();
		const SP<OpenGLElementBuffer>& GetElementBuffer();

		static SP<OpenGLVertexArray> Create();
	private:
		uint32_t m_renderID = 0;
		uint32_t m_vertextBufferIndex = 0;
		std::vector<SP<OpenGLVertextBuffer>> m_vertexBuffers;
		SP<OpenGLElementBuffer> m_elementBuffer;
	};
}
