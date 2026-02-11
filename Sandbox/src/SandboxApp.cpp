#include <Hazel.h>
#include "../vendor/imgui/imgui.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{
		//顶点数据
		float vertices[] = {
			// 位置              // 颜色
			0.0f, 0.0f, 0.0f,  0.8f, 0.2f, 0.8f, // 左下角，红色
			1.0f, 0.0f, 0.0f,  0.2f, 0.3f, 0.8f, // 右下角，绿色
			0.5f,  1.0f, 0.0f,  0.8f, 0.8f, 0.2f  // 顶部，蓝色
		};

		//索引数据
		uint32_t indices[] = { 0, 1, 2 };

		mVertexArray = std::make_shared<Hazel::OpenGLVertexArray>();
		mVertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		//正交相机
		mCamera = std::make_shared<Hazel::OrthographicCamera>(-1.6f, 1.6f, -0.9f, 0.9f);
		

		//设置顶点布局
		{
			Hazel::BufferLayout layout = {
			{"position", Hazel::ShaderDataType::Float3},
			{"color", Hazel::ShaderDataType::Float3}
			};
			mVertexBuffer->SetLayout(layout);
		}
		mVertexArray->AddVertexBuffer(mVertexBuffer);

		mIndexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		mVertexArray->AddIndexBuffer(mIndexBuffer);


		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 aPosition;
			layout(location = 1) in vec3 aColor;
			uniform mat4 viewProjection;
			out vec3 color;
		
			void main()
			{
				color = aColor;
				gl_Position = viewProjection * vec4(aPosition, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 fColor;
			in vec3 color;
		
			void main()
			{
				fColor = vec4(color, 1.0);
			}
		)";

		mShader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));
	}

	virtual void OnUpdate() override
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
			mCameraPosition.x -= mCameraMoveSpeed;
		else if(Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			mCameraPosition.x += mCameraMoveSpeed;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
			mCameraPosition.y += mCameraMoveSpeed;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
			mCameraPosition.y -= mCameraMoveSpeed;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
		{
			mCameraRotation += mCameraRotationSpeed;
		}
		if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
		{
			mCameraRotation -= mCameraRotationSpeed;
		}
		
		mCamera->SetPosition(mCameraPosition);
		mCamera->SetRotation(mCameraRotation);
			
		//OpenGL渲染
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();

		Hazel::Renderer::BeginScene(mCamera);

		Hazel::Renderer::Submit(mVertexArray, mShader);

		Hazel::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		
	}

	virtual void OnEvent(Hazel::Event& event) override
	{
		
	}

private:
	std::shared_ptr<Hazel::Shader> mShader;
	std::shared_ptr<Hazel::VertexArray> mVertexArray;
	std::shared_ptr<Hazel::VertexBuffer> mVertexBuffer;
	std::shared_ptr<Hazel::IndexBuffer> mIndexBuffer;

	std::shared_ptr<Hazel::OrthographicCamera> mCamera;

	glm::vec3 mCameraPosition = { 0.0f, 0.0f, 0.0f };
	float mCameraMoveSpeed = 0.05f;
	float mCameraRotation = 0.0f;
	float mCameraRotationSpeed = 2.0f;
};




class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}
