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


