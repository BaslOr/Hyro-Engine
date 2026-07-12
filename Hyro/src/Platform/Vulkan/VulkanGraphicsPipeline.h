#pragma once
#include <vulkan/vulkan.h>

#include <filesystem>
#include <string>
#include <vector>

#include <vulkan/vulkan_core.h>

#include "Hyro/Renderer/Utils/ShaderUtils.h"


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

		std::pair<VkVertexInputBindingDescription, std::vector<VkVertexInputAttributeDescription>>
			GetBindingAndAttributes(const ShaderReflectionData& reflection);

		std::string ReadFile(const std::string& filepath);
		VkShaderModule CreateShaderModule(const std::vector<uint32_t>& code);

		uint32_t FormatSize(VkFormat format);


	private:
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkPipelineLayout m_PipelineLayout;		
		VkPipeline m_Pipeline;
	};

}