#pragma once
#include "Hyro/Core/Layer.h"
#include <Hyro/Time/TimeStep.h>

namespace Hyro {

	class FPSOverlay : public Layer {
	public:
		FPSOverlay() = default;
		~FPSOverlay() = default;


		void OnAttach() override { }
		void OnDetach() override { }
		void OnUpdate(const TimeStep deltaTime) override;
		void OnImGuiRender() override;
		void OnEvent(Event& event) override { }

	private:
		TimeStep m_AvgInterval = 0.5f;
		uint32_t m_NumFrames = 0;
		TimeStep m_AccumulatedTime = 0.f;
		float m_CurrentFPS = 0;
		float CurrentFPS = 0.f;
	};

}
