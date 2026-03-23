#include <Hazel.h>
#include <../vendor/imgui/imgui.h>
#include <../vendor/glm/gtc/matrix_transform.hpp>
#include <../vendor/glm/gtc/type_ptr.hpp>
//#include <glad/glad.h>
#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer() : Layer("Example"), mPosition(0.0f)
	{
		//顶点数据
		float vertices[] = {
			// 位置              // 纹理坐标
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // 左下角
			0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // 右下角
			-0.5f, 0.5f, 0.0f,   0.0f, 1.0f, //左上角
			0.5f, 0.5f, 0.0f,    1.0f, 1.0f  //右上角
		};

		//索引数据
		uint32_t indices[] = { 0, 1, 2,
							   2, 3, 1 };

		mVertexArray = std::make_shared<Hazel::OpenGLVertexArray>();
		mVertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));

		//正交相机，初始化视图矩阵和投影矩阵
		mCamera = std::make_shared<Hazel::OrthographicCamera>(-1.6f, 1.6f, -0.9f, 0.9f);
		

		//设置顶点布局
		{
			Hazel::BufferLayout layout = {
			{"position", Hazel::ShaderDataType::Float3},
			{"texCoord", Hazel::ShaderDataType::Float2}
			};
			mVertexBuffer->SetLayout(layout);
		}
		mVertexArray->AddVertexBuffer(mVertexBuffer);

		mIndexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		mVertexArray->AddIndexBuffer(mIndexBuffer);


		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 aPosition;
			layout(location = 1) in vec2 aTexCoord;
			out vec2 TexCoord;
			uniform mat4 viewProjection;
			uniform mat4 transform;
			
			void main()
			{
				TexCoord = aTexCoord;
				gl_Position = viewProjection * transform * vec4(aPosition, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 fColor;
			uniform vec3 color;
			uniform sampler2D ourTexture;
			in vec2 TexCoord;
		
			void main()
			{
				fColor = texture(ourTexture, TexCoord);
			}
		)";

		//mShader.reset(Hazel::Shader::Create(vertexSrc, fragmentSrc));
		//mShader = Hazel::Shader::Create("assets/shaders/Texture.glsl");
		//std::dynamic_pointer_cast<Hazel::OpenGLShader>(mShader)->Bind();
		//std::dynamic_pointer_cast<Hazel::OpenGLShader>(mShader)->UploadUniformInt("ourTexture", 0);
		
		auto textureShader = mShaderLibrary.Load("assets/shaders/Texture.glsl");
		
		mTexture = Hazel::Texture2D::Create("assets/textures/myTexture.png");
		mTexture->Bind(0);
	}


	//循环
	virtual void OnUpdate(Hazel::Timestep ts) override
	{
		//相机平移输入
		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
			mCameraPosition.x -= mCameraMoveSpeed * ts;
		else if(Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			mCameraPosition.x += mCameraMoveSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
			mCameraPosition.y += mCameraMoveSpeed * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
			mCameraPosition.y -= mCameraMoveSpeed * ts;

		//相机旋转输入
		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
		{
			mCameraRotation += mCameraRotationSpeed * ts;
		}
		if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
		{
			mCameraRotation -= mCameraRotationSpeed * ts;
		}

		//模型平移输入
		if (Hazel::Input::IsKeyPressed(HZ_KEY_J))
			mPosition.x -= 1.0f * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_L))
			mPosition.x += 1.0f * ts;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_I))
			mPosition.y += 1.0f * ts;
		else if (Hazel::Input::IsKeyPressed(HZ_KEY_K))
			mPosition.y -= 1.0f * ts;
		
		//设置相机的平移和旋转矩阵
		mCamera->SetPosition(mCameraPosition);
		mCamera->SetRotation(mCameraRotation);
			
		//OpenGL渲染
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();

		Hazel::Renderer::BeginScene(mCamera);

		//std::dynamic_pointer_cast<Hazel::OpenGLShader>(mShader)->UploadUniformFloat3("color", color);

		//初始化模型矩阵
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), mPosition);
		transform = glm::scale(transform, glm::vec3(2.0f, 2.0f, 1.0f));

		//设置pvm矩阵并渲染
		auto textureShader = mShaderLibrary.Get("Texture");
		Hazel::Renderer::Submit(mVertexArray, textureShader, transform);

		Hazel::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Color", glm::value_ptr(color));
		ImGui::End();
	}

	virtual void OnEvent(Hazel::Event& event) override
	{
		
	}

private:
	Hazel::ShaderLibrary mShaderLibrary;
	Hazel::Ref<Hazel::Shader> mShader;
	Hazel::Ref<Hazel::VertexArray> mVertexArray;
	Hazel::Ref<Hazel::VertexBuffer> mVertexBuffer;
	Hazel::Ref<Hazel::IndexBuffer> mIndexBuffer;
	Hazel::Ref<Hazel::Texture2D> mTexture;

	Hazel::Ref<Hazel::OrthographicCamera> mCamera;

	glm::vec3 mCameraPosition = { 0.0f, 0.0f, 0.0f };
	float mCameraMoveSpeed = 1.0f;
	float mCameraRotation = 0.0f;
	float mCameraRotationSpeed = 90.0f;

	glm::vec3 mPosition;
	glm::vec3 color = { 1.0f, 1.0f, 1.0f };
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
