#pragma once

#include "Window.h"
#include "Event/Event.h"
#include "Hazel/LayerStack.h"
#include "Hazel/Event/ApplicationEvent.h"

#include "Hazel/ImGui/ImGuiLayer.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"

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

		//unsigned int mVertexArray;
		//unsigned int mVertexBuffer;

		std::shared_ptr<Shader> mShader;
		std::shared_ptr<VertexArray> mVertexArray;
		std::shared_ptr<VertexBuffer> mVertexBuffer;
		std::shared_ptr<IndexBuffer> mIndexBuffer;
		//unsigned int mIndexBuffer;
	};
	
	Application* CreateApplication();
}



