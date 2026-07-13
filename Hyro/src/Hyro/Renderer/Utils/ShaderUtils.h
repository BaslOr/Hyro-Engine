#pragma once
#include <filesystem>
#include <vector>

#include <spirv_reflect.h>
#include <Hyro/Renderer/Vertex.h>
#include "Hyro/Renderer/Shader.h"
#include <cstdint>


namespace Hyro {

	class ShaderUtils {
	public:
		static uint32_t GetCountFromShaderType(ShaderType type);

	};

	class ShaderCompiler {
	public:
		static std::vector<uint32_t> CompileToSpirv(const std::filesystem::path& sourcePath, ShaderStage stage);

	private:
		static std::filesystem::path GetGlslcPath();
		static const char* StageToFlag(ShaderStage stage);
	};


	struct ShaderReflectionData {

		struct Descriptor {
			uint32_t Set;
			uint32_t Binding;
			DescriptorType Type;
			uint32_t Count;
			ShaderStage Stage;
		};

		std::vector<Descriptor> Descriptors;

		struct PushConstant {
			uint32_t Offset;
			uint32_t Size;
			ShaderStage Stage;
		};

		std::vector<PushConstant> PushConstants;
	};



	class ShaderReflection {
	public:
		static VertexLayout GetVertexLayout(const std::vector<uint32_t>& vertexSpirV);
		static ShaderReflectionData FillReflectionData(const std::array<std::vector<uint32_t>, 2>& spirVs);

	private:
		static ShaderType ReflectTypeToGLType(SpvReflectFormat format);
		static DescriptorType SpvDescriptorTypeToHyroType(SpvReflectDescriptorType type);

	};

}