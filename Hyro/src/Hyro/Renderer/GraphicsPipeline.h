#pragma once
#include <string>
#include <cstdint>

namespace Hyro {
	
	struct DepthInfo {
		//Aligns with Vulkan depth compare operations
		enum class CompareOp {
			Never = 0,
			Less = 1,
			Equal = 2,
			LessEqual = 3,
			Greater = 4,
			NotEqual = 5,
			GreaterEqual = 6,
			Always = 7,
		};

		bool DepthTest = true;
		bool DepthWrite = true;
		CompareOp DepthFunc = CompareOp::Less;
	};

}