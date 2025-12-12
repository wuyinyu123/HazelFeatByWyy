#pragma once

#include "Hazel/Core.h"

namespace Hazel
{
	class HAZEL_API Input
	{
	public:
		static bool IsKeyPressed(int keycode)
		{
			return sInstance->IsKeyPressedImpl(keycode);
		}


		static bool IsMouseButtonPressed(int button)
		{
			return sInstance->IsMouseButtonPressedImpl(button);
		}

		static std::pair<float, float> GetMousePosition() 
		{
			return sInstance->GetMousePositionImpl();
		}

		static float GetMouseX()
		{
			return sInstance->GetMouseXImpl();
		}

		static float GetMouseY()
		{
			return sInstance->GetMouseYImpl();
		}

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() const = 0;
		virtual float GetMouseXImpl() const = 0;
		virtual float GetMouseYImpl() const = 0;

	private:
		static Input* sInstance;
	};
}