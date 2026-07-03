#pragma once

#include <vulkan/vulkan.h>


namespace Hyro {

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

		std::vector<char> ReadFile(const std::string& filepath);
		VkShaderModule CreateShaderModule(const std::vector<char>& code);

	private:
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkPipelineLayout m_PipelineLayout;		
		VkPipeline m_Pipeline;
	};

}