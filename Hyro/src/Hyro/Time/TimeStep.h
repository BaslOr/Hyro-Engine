#pragma once

namespace Hyro {

	class TimeStep {
	public:
		TimeStep() 
			: m_TimeStep(0.f)
		{ }
		TimeStep(float timeStep)
			: m_TimeStep(timeStep) 
		{ }

		~TimeStep() = default;

		inline operator float() { return m_TimeStep; }
		inline operator float() const { return m_TimeStep; }

	private:
		float m_TimeStep;
	};

}
