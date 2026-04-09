#pragma once
//------------Entrypoint------------------------------
#include <Hyro/Core/entrypoint.h>
//----------------------------------------------------

#include "SandboxLayer.h"

class WaterSimulation : public Hyro::Application {
public:
	WaterSimulation()
		: Application("Water Simulation", 1280, 720)
	{
		auto window = GetWindow();
		PushLayer(new SandboxLayer(window));
	}
	~WaterSimulation() = default;


private:
};

Hyro::Application* CreateApplication() {
	return new WaterSimulation;
}
