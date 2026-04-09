#pragma once
#include <string>

#include "Hyro/Events/Event.h"
#include "Hyro/Time/TimeStep.h"

namespace Hyro {

	class Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate(const TimeStep deltaTime) = 0;
		virtual void OnImGuiRender() = 0;
		virtual void OnEvent(Event& event) = 0;

		inline const std::string& GetName() { return m_DebugName; }

	private:
		std::string m_DebugName;
	};

}
