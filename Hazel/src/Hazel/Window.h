#pragma once

#include "hzpch.h"
#include "Hazel/Core.h"
#include "Hazel/Event/Event.h"


namespace Hazel
{
	struct WindowProps
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "Hazel Engine",
			unsigned int width = 1280, unsigned int height = 720)
			: title(title), width(width), height(height)
		{

		}
	};

	class HAZEL_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>; //使用using起别名

		virtual ~Window()
		{

		};

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		
		//根据不同平台获取不同的指针
		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}