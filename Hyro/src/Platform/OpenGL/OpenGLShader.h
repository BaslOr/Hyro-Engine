#pragma once
#include "Hyro/Renderer/Shader.h"
#include <iostream>
#include <unordered_map>
#include <string>

#include "Hyro/Renderer/Utils/ShaderUtils.h"


namespace Hyro {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertexPath, const std::string& fragPath);
		~OpenGLShader();

		void Bind() const override;
		void Bind(void* commandBuffer) const override;

		VertexLayout GetVertexLayout() const override { return m_VertexLayout; }

		uint32_t GetProgram() const { return m_Program; }

		void SetUniformInt(const std::string& name, int vlaue) const;
		void SetUniformFloat(const std::string& name, float value) const;
		void SetUnifromBool(const std::string& name, bool value) const;
		void setUniformVec3(const std::string& name, const glm::vec3& value) const;
		void SetUniformVec4(const std::string& name, const glm::vec4& value) const;
		void SetUniformMat4(const std::string& name, const glm::mat4& value) const;


	private:
		friend class OpenGLMaterial;

		void CheckShaderCompilation(uint32_t shader);
		void CheckLinkingErrors();

		uint32_t CreateShader(uint32_t shaderType, const char* shaderSource);

		void RetrieveVertexLayout(const std::string& vertexPath);
		VertexAttributeType ReflectTypeToGLType(SpvReflectFormat format) const;

		int GetUniformLocation(const std::string& name) const;
		bool CheckLocation(int location) const;

		std::string ReadShaderFromFile(const std::string& filePath);

	private:
		uint32_t m_Program;
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;

		VertexLayout m_VertexLayout;
	};

}
