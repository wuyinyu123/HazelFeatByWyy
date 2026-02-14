#include "hzpch.h"
#include "Renderer.h"

namespace Hazel
{
    Renderer::SceneData* Renderer::mSceneData = new Renderer::SceneData();

    void Renderer::BeginScene(const std::shared_ptr<OrthographicCamera>& camera)
    {
        mSceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {

    }

    void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4 transform)
    {
        shader->Bind();
        shader->UploadUniformMat4("viewProjection", mSceneData->ViewProjectionMatrix);
        shader->UploadUniformMat4("transform", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    
}