#pragma once
#include "Hyro/Renderer/Shader.h"
#include <iostream>
#include <unordered_map>
#include <string>

#include "Hyro/Renderer/Utils/ShaderUtils.h"

namespace Hyro {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const DepthInfo& depthInfo, const std::string& vertexPath, const std::string& fragPath);
		~OpenGLShader();

		void Bind() const override;
		void Bind(void* commandBuffer) const override;

		VertexLayout GetVertexLayout() const override { return m_VertexLayout; }
		ShaderReflectionData GetReflectionData() const override { return m_ReflectionData; }

		uint32_t GetProgram() const { return m_Program; }

		void SetUnifrom(Uniform uniform);


	private:
		friend class OpenGLMaterial;

		int GetUniformLocation(const std::string& name) const;
		bool CheckLocation(int location) const;

		void CheckShaderCompilation(uint32_t shader);
		void CheckLinkingErrors();

		static uint32_t HyroCompareOpToOpenGLDepthFunc(DepthInfo::CompareOp compareOp);

		uint32_t CreateShader(uint32_t shaderType, const char* shaderSource);
		std::string ReadShaderFromFile(const std::string& filePath);

	private:
		uint32_t m_Program;
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;

		VertexLayout m_VertexLayout;
		ShaderReflectionData m_ReflectionData;

		DepthInfo m_DepthInfo;
	};

}
