#pragma once
#include "Hyro/Core/Logger.h"

#define BIT(x) (1 << x)
#define HYRO_ASSERT(x) assert((x))
#define HYRO_STATIC_ASSERT(x) static_assert((x))


//Define log macros
#ifdef HYRO_DEBUG

#define HYRO_LOG_CORE_TRACE(...)      Hyro::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define HYRO_LOG_CORE_INFO(...)       Hyro::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define HYRO_LOG_CORE_WARN(...)       Hyro::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define HYRO_LOG_CORE_ERROR(...)      Hyro::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define HYRO_LOG_CORE_FATAL(...)      Hyro::Logger::GetCoreLogger()->error(__VA_ARGS__)

#define HYRO_LOG_CLIENT_TRACE(...)    Hyro::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define HYRO_LOG_CLIENT_INFO(...)     Hyro::Logger::GetClientLogger()->info(__VA_ARGS__)
#define HYRO_LOG_CLIENT_WARN(...)     Hyro::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define HYRO_LOG_CLIENT_ERROR(...)    Hyro::Logger::GetClientLogger()->error(__VA_ARGS__)
#define HYRO_LOG_CLIENT_FATAL(...)    Hyro::Logger::GetClientLogger()->error(__VA_ARGS__)

#else 

#define HYRO_LOG_CORE_TRACE(...)  
#define HYRO_LOG_CORE_INFO(...)   
#define HYRO_LOG_CORE_WARN(...)   
#define HYRO_LOG_CORE_ERROR(...)  
#define HYRO_LOG_CORE_FATAL(...)  

#define HYRO_LOG_CLIENT_TRACE(...)
#define HYRO_LOG_CLIENT_INFO(...) 
#define HYRO_LOG_CLIENT_WARN(...) 
#define HYRO_LOG_CLIENT_ERROR(...)
#define HYRO_LOG_CLIENT_FATAL(...)

#endif // HYRO_DEBUG



namespace Hyro {

	enum class BuildConfig {
		Debug = 0,
		Release = 1,
		Distribution = 2
	};

#ifdef HYRO_DEBUG
	constexpr BuildConfig CurrentBuildConfig = BuildConfig::Debug;
#elif defined(HYRO_RELEASE)
	constexpr BuildConfig CurrentBuildConfig = BuildConfig::Release;
#elif defined(HYRO_DIST)
	constexpr BuildConfig CurrentBuildConfig = BuildConfig::Distribution;
#endif // HYRO_DEBUG
	
}

