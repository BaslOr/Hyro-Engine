#pragma once
#include <vulkan/vulkan.h>

#include <string>
#include <vector>
#include <array>

#include "Hyro/Renderer/Utils/ShaderUtils.h"
#include "Hyro/Renderer/Vertex.h"
#include <cstdint>
#include <Hyro/Renderer/Shader.h>


namespace Hyro {

	class VulkanGraphicsPipeline {
	public:
		VulkanGraphicsPipeline(const std::string& vertexPath, const std::string& fragmentPath);
		~VulkanGraphicsPipeline();

		inline VkPipeline GetVkPipeline() const { return m_Pipeline; }
		inline VkDescriptorSetLayout GetVkDescriptorSetLayout() const { return m_DescriptorSetLayout; }
		inline VkPipelineLayout GetVkPipelineLayout() const { return m_PipelineLayout; }

		inline VertexLayout GetVertexLayout() const { return m_VertexLayout; }
		inline ShaderReflectionData GetReflectionData() const { return m_ReflectionData; }

	private:
		void CreatePipeline(const std::string& vertexPath, const std::string& fragmentPath);
		void CreateDescriptorSetLayout(const ShaderReflectionData& reflection);

		std::pair<VkVertexInputBindingDescription, std::vector<VkVertexInputAttributeDescription>>
			GetBindingAndAttributes(const VertexLayout& layout);
		std::vector<VkPushConstantRange> RetrievePushConstants(const ShaderReflectionData& data) const;

		std::string ReadFile(const std::string& filepath);
		VkShaderModule CreateShaderModule(const std::vector<uint32_t>& code);

		static VkDescriptorType HyroDescriptorTypeToVulkanType(DescriptorType type);
		static VkShaderStageFlags HyroShaderStageToVulkanStage(ShaderStage stage);
		static VkFormat HyroFormatToVulkanFormat(VertexAttributeType format);
		static VertexAttributeType VkTypeToHyroType(VkFormat format);
		static uint32_t FormatSize(VkFormat format);

	private:
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkPipelineLayout m_PipelineLayout;		
		VkPipeline m_Pipeline;

		VertexLayout m_VertexLayout;
		ShaderReflectionData m_ReflectionData;
	};

}