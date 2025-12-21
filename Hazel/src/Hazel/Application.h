#pragma once

#include "Window.h"
#include "Event/Event.h"
#include "Hazel/LayerStack.h"
#include "Hazel/Event/ApplicationEvent.h"

#include "Hazel/ImGui/ImGuiLayer.h"

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
		ImGuiLayer* mImGuiLayer;
		bool mRunning = true;
		LayerStack mLayerStack;

		bool OnWindowClosed(WindowCloseEvent& e);

	private:
		static Application* sInstance;
	};
	
	Application* CreateApplication();
}



