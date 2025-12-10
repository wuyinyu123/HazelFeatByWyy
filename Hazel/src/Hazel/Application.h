#pragma once

#include "Window.h"
#include "Event/Event.h"
#include "Hazel/LayerStack.h"
#include "Hazel/Event/ApplicationEvent.h"

namespace Hazel
{
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get()
		{
			return *sInstance;
		}

		inline Window& GetWindow()
		{
			return *mWindow;
		}
	private:
		std::unique_ptr<Window> mWindow;
		bool mRunning = true;
		
		bool OnWindowClosed(WindowCloseEvent& e);
		LayerStack mLayerStack;
		static Application* sInstance;
	};
	
	Application* CreateApplication();
}



