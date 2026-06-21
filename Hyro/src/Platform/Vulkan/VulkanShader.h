#pragma once
#include "Hyro/Renderer/Shader.h"

#include "Platform/Vulkan/VulkanBuffer.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"


namespace Hyro {

	class VulkanShader : public Shader {
	public: 
		VulkanShader(const GraphicsPipelineSettings& settings);
		~VulkanShader() = default;

		void Bind() const override;
		void Bind(void* commandBuffer) const override;

		void SetUniformInt(const std::string& name, int vlaue) const override {}
		void SetUniformFloat(const std::string& name, float value) const override {}
		void SetUnifromBool(const std::string& name, bool value) const override {}
		void setUniformVec3(const std::string& name, const glm::vec3& value) const override {}
		void SetUniformVec4(const std::string& name, const glm::vec4& value) const override {}
		void setUniformMat4(const std::string& name, const glm::mat4& value) const override;

	private:
		Ref<VulkanGraphicsPipeline> m_Pipeline;
		Ref<UniformBuffer> m_UniformBuffer;
	};
}