#include "hzpch.h"

#include "Hazel/Log.h"
#include "Event/Event.h"
#include "Application.h"

#include "Input.h"

#include <glad/glad.h>
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Hazel
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::sInstance = nullptr;


	

	Application::Application()  
	{
		HZ_CORE_ASSERT(!sInstance, "Application already exists!");
		sInstance = this;

		//初始化窗口并设置事件回调
		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));

		mImGuiLayer = new ImGuiLayer();
		PushOverlay(mImGuiLayer);

		float vertices[] = {
			// 位置              // 颜色
			0.0f, 0.0f, 0.0f,  0.8f, 0.2f, 0.8f, // 左下角，红色
			1.0f, 0.0f, 0.0f,  0.2f, 0.3f, 0.8f, // 右下角，绿色
			0.5f,  1.0f, 0.0f,  0.8f, 0.8f, 0.2f  // 顶部，蓝色
		};

		uint32_t indices[] = {0, 1, 2};

		mVertexArray = std::make_shared<OpenGLVertexArray>();
		
		mVertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		
		//设置顶点布局
		{
			BufferLayout layout = {
			{"position", ShaderDataType::Float3},
			{"color", ShaderDataType::Float3}
			};
			mVertexBuffer->SetLayout(layout);
		}
		mVertexArray->AddVertexBuffer(mVertexBuffer);
		
		mIndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		mVertexArray->AddIndexBuffer(mIndexBuffer);
		

		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 aPosition;
			layout(location = 1) in vec3 aColor;
			out vec3 color;
		
			void main()
			{
				color = aColor;
				gl_Position = vec4(aPosition, 1.0);
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

		mShader.reset(new Shader(vertexSrc, fragmentSrc));
	}

	Application::~Application()
	{
		
	}

	void Application::PushLayer(Layer* layer)
	{
		mLayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		mLayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));

		//HZ_CORE_TRACE("{0}", e);

		for (auto it = mLayerStack.end(); it != mLayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.mHandled)
			{
				break;
			}
		}
	}

	void Application::Run()
	{
		WindowResizeEvent w(1280, 720);
		HZ_TRACE(w);

		while (mRunning)
		{
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			mShader->Bind();
			mVertexArray->Bind();
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

			for (Layer* layer : mLayerStack)
				layer->OnUpdate();

			mImGuiLayer->Begin();
			for (Layer* layer : mLayerStack)
				layer->OnImGuiRender();
			mImGuiLayer->End();

			mWindow->OnUpdate();
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		mRunning = false;
		return true;
	}

	
}


