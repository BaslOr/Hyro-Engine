#pragma once
#include <filesystem>
#include <vector>

#include <spirv_reflect.h>


namespace Hyro {

	enum class ShaderStage {
		Vertex,
		Fragment,
		Compute
	};

	class ShaderCompiler {
	public:
		static std::vector<uint32_t> CompileToSpirv(const std::filesystem::path& sourcePath, ShaderStage stage);

	private:
		static std::filesystem::path GetGlslcPath();
		static const char* StageToFlag(ShaderStage stage);
	};



	struct ShaderReflectionData {
		std::vector<SpvReflectInterfaceVariable*> VertexInputs;
		std::vector<SpvReflectDescriptorSet*> DescriptorSets;
		std::vector<SpvReflectBlockVariable*> PushConstants;
	};

	class ShaderReflection {
	public:
		static ShaderReflectionData FillReflectionData(ShaderStage stage, const std::vector<uint32_t>& bytes);
	};

}