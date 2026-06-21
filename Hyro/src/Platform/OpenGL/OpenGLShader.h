#pragma once
#include "Hyro/Renderer/Shader.h"
#include <iostream>
#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>

namespace Hyro {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& vertexPath, const std::string& fragPath);
		~OpenGLShader();

		void Bind() const override;
		void Bind(void* commandBuffer) const override;

		void SetUniformInt(const std::string& name, int vlaue) const override;
		void SetUniformFloat(const std::string& name, float value) const override;
		void SetUnifromBool(const std::string& name, bool value) const override;
		void setUniformVec3(const std::string& name, const glm::vec3& value) const override;
		void SetUniformVec4(const std::string& name, const glm::vec4& value) const override;
		void setUniformMat4(const std::string& name, const glm::mat4& value) const override;

	private:
		uint32_t m_Program;
		mutable std::unordered_map<std::string, int> m_UniformLocationCache;

		int getUniformLocation(const std::string& name) const;

		void checkShaderCompilation(uint32_t shader);
		void checkLinkingErrors();
		void checkLocation(int location) const;

		uint32_t createShader(uint32_t shaderType, const char* shaderSource);

		std::string readShaderFromFile(const std::string& filePath);

	};

}
