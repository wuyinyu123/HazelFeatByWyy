#include "hzpch.h"

#include "Hazel/Log.h"
#include "Event/Event.h"
#include "Application.h"

#include "Input.h"

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
		//事件处理，如果已经被前面的层处理则后续的层不会处理该事件
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
			//每个层的更新逻辑
			for (Layer* layer : mLayerStack)
				layer->OnUpdate();

			//imGui渲染
			mImGuiLayer->Begin();
			for (Layer* layer : mLayerStack)
				layer->OnImGuiRender();
			mImGuiLayer->End();

			//处理事件队列
			mWindow->OnUpdate();
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		mRunning = false;
		return true;
	}

	
}


