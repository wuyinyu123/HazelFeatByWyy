#include "hzpch.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case Hazel::RendererAPI::API::None:
			HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;

		case Hazel::RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}


