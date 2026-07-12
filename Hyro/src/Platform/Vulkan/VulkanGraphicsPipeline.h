#pragma once
#include <vulkan/vulkan.h>

#include <filesystem>
#include <string>
#include <vector>

#include <spirv_reflect.h>
#include <vulkan/vulkan_core.h>


namespace Hyro {


	//Move in some kind of ShaderUtils file
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

		

	class VulkanGraphicsPipeline {
	public:
		VulkanGraphicsPipeline(const std::string& vertexPath, const std::string& fragmentPath);
		~VulkanGraphicsPipeline();

		inline VkPipeline GetVkPipeline() const { return m_Pipeline; }
		inline VkDescriptorSetLayout GetVkDescriptorSetLayout() const { return m_DescriptorSetLayout; }
		inline VkPipelineLayout GetVkPipelineLayout() const { return m_PipelineLayout; }

	private:
		void CreatePipeline(const std::string& vertexPath, const std::string& fragmentPath);
		void CreateDescriptorSetLayout();

		std::pair<VkVertexInputBindingDescription, std::vector<VkVertexInputAttributeDescription>>
			GetBindingAndAttributes(const ShaderReflectionData& reflection);

		std::string ReadFile(const std::string& filepath);
		VkShaderModule CreateShaderModule(const std::vector<uint32_t>& code);

	private:
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkPipelineLayout m_PipelineLayout;		
		VkPipeline m_Pipeline;
	};

}