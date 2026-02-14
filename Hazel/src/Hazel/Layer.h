#pragma once

#include "Hazel/Core.h"
#include "Hazel/Event/Event.h"
#include "Hazel/Core/Timestep.h"

namespace Hazel
{
	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach(){ }
		virtual void OnDetach(){ }
		virtual void OnUpdate(Timestep time){ }
		virtual void OnEvent(Event& event){ }
		virtual void OnImGuiRender(){ }

		inline const std::string& GetName() const
		{
			return mDebugName;
		}

	protected:
		std::string mDebugName;
	};
}