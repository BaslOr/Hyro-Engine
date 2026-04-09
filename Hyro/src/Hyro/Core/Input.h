#pragma once
#include "Hyro/Core/KeyCodes.h"
#include "Hyro/Core/MouseCodes.h"

#include "Hyro/Core/Memory.h"

namespace Hyro {

	class Input {
	public:
		static inline bool IsKeyPressed(int keyCode) { return s_Instance->IsKeyPressedImpl(keyCode); }

		static inline bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		static inline float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		static inline float GetMouseY() { return s_Instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int keyCode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Scope<Input> s_Instance;
	};

}
