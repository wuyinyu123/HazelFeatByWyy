#pragma once

#include "Hazel/Window.h"

#include "GLFW/glfw3.h"



namespace Hazel
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);

		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override
		{
			return mData.Width;
		}

		inline unsigned int GetHeight() const override
		{
			return mData.Height;
		}

		inline void SetEventCallback(const EventCallbackFn& callback) override
		{
			mData.EventCallback = callback;
		}

		inline void* GetNativeWindow() const
		{
			return mWindow;
		}

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* mWindow;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData mData;
	};
}