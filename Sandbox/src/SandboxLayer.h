#pragma once
#include <Hyro.h>

class SandboxLayer : public Hyro::Layer {
public:
	SandboxLayer(Hyro::Ref<Hyro::Window> window);
	~SandboxLayer() = default;

	virtual void OnUpdate(const Hyro::TimeStep deltaTime) override;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Hyro::Event& event) override {};

private:
	Hyro::Ref<Hyro::Renderer2D> m_Renderer;

};
