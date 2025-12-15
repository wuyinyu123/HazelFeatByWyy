#pragma once

#include "Hazel/Layer.h"

#include "Hazel/Event/Event.h"
#include "Hazel/Event/ApplicationEvent.h"
#include "Hazel/Event/KeyEvent.h"
#include "Hazel/Event/MounseEvent.h"

namespace Hazel
{
	class HAZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnImGuiRender() override;
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
		
	private:
		float mTime = 0.0f;
	};
}



