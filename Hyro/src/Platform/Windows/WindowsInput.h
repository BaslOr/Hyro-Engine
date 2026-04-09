#pragma once
#include "Hyro/Core/Input.h"

namespace Hyro {

	class WindowsInput : public Input {
	protected:
		virtual bool IsKeyPressedImpl(int keyCode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;

	};

}
