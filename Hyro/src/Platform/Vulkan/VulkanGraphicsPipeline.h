#pragma once
#include <vulkan/vulkan.h>

#include <filesystem>
#include <string>
#include <vector>


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

		std::string ReadFile(const std::string& filepath);
		VkShaderModule CreateShaderModule(const std::vector<uint32_t>& code);

	private:
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkPipelineLayout m_PipelineLayout;		
		VkPipeline m_Pipeline;
	};

}