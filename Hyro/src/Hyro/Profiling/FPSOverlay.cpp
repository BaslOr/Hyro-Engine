#include "pch.h"
#include "Hyro/Profiling/FPSOverlay.h"

#include <imgui.h>

namespace Hyro {

	void FPSOverlay::OnUpdate(const TimeStep deltaTime)
	{
		++m_NumFrames;
		m_AccumulatedTime += deltaTime;

		if (m_AccumulatedTime > m_AvgInterval) {
			m_CurrentFPS = static_cast<float>(m_NumFrames) / m_AccumulatedTime;

			m_AccumulatedTime = 0;
			m_NumFrames = 0;
		}
	}

	void FPSOverlay::OnImGuiRender()
	{

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(
			{
				viewport->WorkPos.x + viewport->WorkSize.x - 15.0f,
				viewport->WorkPos.y + 15.0f
			},
			ImGuiCond_Always,
			{ 1.0f, 0.0f }
		);

			ImGui::SetNextWindowBgAlpha(0.3f);
			ImGui::SetNextWindowSize(ImVec2(ImGui::CalcTextSize("FPS: ______").x, 0));

			if (ImGui::Begin("##FPS", nullptr,
				ImGuiWindowFlags_NoDecoration |
				ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoFocusOnAppearing |
				ImGuiWindowFlags_NoNav |
				ImGuiWindowFlags_NoMove))
			{
				ImGui::Text("FPS: %i", static_cast<int>(m_CurrentFPS));
				ImGui::Text("ms: %.1f", 1000.0f / m_CurrentFPS);

				ImGui::End();
			}
	}

}
