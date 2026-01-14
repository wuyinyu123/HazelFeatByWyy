#include "hzpch.h"

#include "Hazel/Log.h"
#include "Event/Event.h"
#include "Application.h"

#include "Input.h"

#include <glad/glad.h>

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
			-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // 左下角，红色
			0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // 右下角，绿色
			0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // 顶部，蓝色
		};

		uint32_t indices[] = {0, 1, 2};

		glGenVertexArrays(1, &mVertexArray); // 生成一个 VAO
		glBindVertexArray(mVertexArray);     // 绑定它，之后的操作都会记录到这个 VAO 中

		mVertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		//glGenBuffers(1, &mVertexBuffer); // 生成一个 VBO
		//glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer); // 绑定到 GL_ARRAY_BUFFER 目标
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// 解析位置属性 (对应顶点着色器中的 location = 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0); // 启用 location 0
		// 解析颜色属性 (对应顶点着色器中的 location = 1)
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1); // 启用 location 1

		mIndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		//glGenBuffers(1, &mIndexBuffer);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		std::string vertexSrc = R"(
			#version 330 core
			layout(location = 0) in vec3 aPosition;
			layout(location = 1) in vec3 aColor;
			out vec3 color;
		
			void main()
			{
				color = aColor;
				glPosition = vec4(aPosition, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 fragColor;
			in vec3 color;
		
			void main()
			{
				fragColor = vec4(color, 1.0);
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
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			mShader->Bind();
			glBindVertexArray(mVertexArray);
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


