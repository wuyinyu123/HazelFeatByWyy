#pragma once
#include "RenderCommand.h"
#include "VertexArray.h"
#include "Hazel/Renderer/Camera.h"
#include "Hazel/Renderer/Shader.h"
#include "../../vendor/glm/glm.hpp"

namespace Hazel
{
	

	class Renderer
	{
	public:
		inline static RendererAPI::API GetAPI()
		{
			return RendererAPI::GetAPI();
		}

	public:
		static void BeginScene(const Ref<OrthographicCamera>& camera);
		static void EndScene();
		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4 transform = 1.0f);
		


		struct SceneData
		{
			
			glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
			
		};
		static SceneData* mSceneData;
	};
}