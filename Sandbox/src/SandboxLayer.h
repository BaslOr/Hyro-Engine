#pragma once
#include <Hyro.h>

class SandboxLayer : public Hyro::Layer {
public:
	SandboxLayer();
	~SandboxLayer() = default;

	virtual void OnUpdate(const Hyro::TimeStep deltaTime) override;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hyro::Event& event) override {};

};
