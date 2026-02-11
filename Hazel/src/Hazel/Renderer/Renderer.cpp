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

    void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader)
    {
        shader->Bind();
        shader->UploadUniformMat4("viewProjection", mSceneData->ViewProjectionMatrix);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }

    
}