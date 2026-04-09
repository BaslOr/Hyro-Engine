#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Hyro/Core/Memory.h"

namespace Hyro {

	class Logger {
	public:
		static void Init();

		static const Ref<spdlog::logger>& GetCoreLogger()   { return s_CoreLogger; }
		static const Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};

}
