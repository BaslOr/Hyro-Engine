#pragma once
#include "Hyro/Core/Layer.h"

namespace Hyro {

	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override {}
		virtual void OnEvent(Event& event) override {}
		virtual void OnUpdate(const TimeStep deltaTime) override {}

		void Begin();
		void End();

	private:

	};

}
