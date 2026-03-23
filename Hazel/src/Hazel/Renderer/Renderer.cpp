#include "hzpch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
    Renderer::SceneData* Renderer::mSceneData = new Renderer::SceneData();

    void Renderer::Init()
    {
        RenderCommand::Init();
    }

    void Renderer::BeginScene(const Ref<OrthographicCamera>& camera)
    {
        mSceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {

    }

    //绑定着色器与顶点数组后调用DrawCall
    void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4 transform)
    {
        shader->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("viewProjection", mSceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("transform", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}