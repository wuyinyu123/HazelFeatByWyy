#pragma once

#include "Hazel/Core.h"
#include "spdlog/fmt/fmt.h"




namespace Hazel
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0), //1 << 0 左移0 得1
		EventCategoryInput = BIT(1), //1 << 1 左移1 得2
		EventCategoryKeyboard = BIT(2), //1 << 2 左移2 得4
		EventCategoryMouse = BIT(3), //1 << 3 左移3 得8
		EventCategoryMouseButton = BIT(4), //1 << 4 左移4 得16
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::##type;}\
								virtual EventType GetEventType() const override {return GetStaticType();}\
								virtual const char* GetName() const override {return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}
	
	class HAZEL_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const
		{
			return GetName();
		}

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
		bool mHandled = false;

	protected:
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
		
	public:
		EventDispatcher(Event& event) : mEvent(event)
		{

		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (mEvent.GetEventType() == T::GetStaticType())
			{
				mEvent.mHandled = func(*(T*)&mEvent);
				return true;
			}
			return false;
		}

	private:
		Event& mEvent;
	};

	
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}

//格式化特化
namespace fmt
{
	template <>
	struct formatter<Hazel::Event> : formatter<std::string>
	{
		auto format(const Hazel::Event& e, format_context& ctx) const
		{
			return formatter<std::string>::format(e.ToString(), ctx);
		}
	};
}
