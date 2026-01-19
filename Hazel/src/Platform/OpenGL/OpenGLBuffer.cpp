#include "hzpch.h"
#include "OpenGLBuffer.h"
#include "glad/glad.h"

namespace Hazel
{
	/// <summary>
	/// VertexBuffer
	/// </summary>
	/// <param name="vertices"></param>
	/// <param name="size"></param>
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) :
		mRendererID(0)
	{
		glCreateBuffers(1, &mRendererID); //与Shader一样使用Create目的是保持API的一致性
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &mRendererID);
	}
	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, mRendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	
	/// <summary>
	/// IndexBuffer
	/// </summary>
	/// <param name="indices"></param>
	/// <param name="count"></param>
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : mRendererID(0), mCount(count)
	{
		glCreateBuffers(1, &mRendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &mRendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t OpenGLIndexBuffer::GetCount() const
	{
		return mCount;
	}
}