#pragma once
#include "../vendor/glm/glm.hpp"


namespace Hazel
{
	// 前向声明 VertexArray
	class VertexArray;

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};

	public:
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI()
		{
			return sAPI;
		}

	private:
		static API sAPI;
	};
}
