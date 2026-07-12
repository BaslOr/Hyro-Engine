#include "pch.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glad/glad.h>

#include "Hyro/Core/Core.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

namespace Hyro {

	OpenGLShader::OpenGLShader(const std::string& vertexPath, const std::string& fragPath)
		: m_Program(0)
	{
		std::string vertexSource = ReadShaderFromFile(vertexPath);
		std::string fragmentSource = ReadShaderFromFile(fragPath);

		const char* vertexShaderSourceCStr = vertexSource.c_str();
		const char* fragmentShaderSourceCStr = fragmentSource.c_str();
		uint32_t vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderSourceCStr);
		uint32_t fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderSourceCStr);

		m_Program = glCreateProgram();
		glAttachShader(m_Program, vertexShader);
		glAttachShader(m_Program, fragmentShader);
		glLinkProgram(m_Program);
		CheckLinkingErrors();

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		RetrieveVertexLayout(vertexPath);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_Program);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_Program);
	}

	void OpenGLShader::Bind(void* commandBuffer) const
	{
		HYRO_LOG_CORE_WARN("Tried to bind Shader with Command Buffer. This may indicate a bug.");
	}

	void OpenGLShader::SetUniformInt(const std::string& name, int value) const
	{
		Bind();
		int location = GetUniformLocation(name);
		glUniform1i(location, value);
	}

	void OpenGLShader::SetUniformFloat(const std::string& name, float value) const
	{
		Bind();
		int location = GetUniformLocation(name);
		glUniform1f(location, value);
	}

	void OpenGLShader::SetUnifromBool(const std::string& name, bool value) const
	{
		Bind();
		int location = GetUniformLocation(name);
		glUniform1i(location, (int)value);
	}

	void OpenGLShader::setUniformVec3(const std::string& name, const glm::vec3& value) const
	{
		Bind();
		int location = GetUniformLocation(name);
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetUniformVec4(const std::string& name, const glm::vec4& value) const
	{
		Bind();
		int location = GetUniformLocation(name);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& value) const
	{
		Bind();
		int location = GetUniformLocation(name);
		glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
	}

	void OpenGLShader::CheckShaderCompilation(uint32_t shader)
	{
		int  success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			HYRO_LOG_CORE_ERROR("Shader compilation failed! \n {}", infoLog);
		}
	}

	void OpenGLShader::CheckLinkingErrors()
	{
		int success;
		char infoLog[512];
		glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
			HYRO_LOG_CORE_ERROR("Shader linking failed! \n {}", infoLog);
		}
	}

	uint32_t OpenGLShader::CreateShader(uint32_t shaderType, const char* shaderSource)
	{
		uint32_t shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &shaderSource, NULL);
		glCompileShader(shader);
		CheckShaderCompilation(shader);

		return shader;
	}

	void OpenGLShader::RetrieveVertexLayout(const std::string& vertexPath)
	{
		//Prbly not the most efficient but I think it's fine
		std::vector<uint32_t> spirV = ShaderCompiler::CompileToSpirv(vertexPath, ShaderStage::Vertex);
		ShaderReflectionData data = ShaderReflection::FillReflectionData(ShaderStage::Vertex, spirV);

		std::vector<std::pair<VertexAttributeType, uint32_t>> buffer;
		for (const auto& attribute : data.VertexInputs) {
			VertexAttributeType type = ReflectTypeToGLType(attribute->format);
			buffer.push_back(std::make_pair(type, attribute->location));
		}

		std::sort(std::begin(buffer), std::end(buffer),
			[](const std::pair<VertexAttributeType, uint32_t>& a, const std::pair<VertexAttributeType, uint32_t>& b) {
				return a.second < b.second;
			});

		for (const auto& [type, location] : buffer) {
			m_VertexLayout.Push(type);
		}
	}

	VertexAttributeType OpenGLShader::ReflectTypeToGLType(SpvReflectFormat format) const
	{
		switch (format)
		{
		case SPV_REFLECT_FORMAT_UNDEFINED:
			break;
		case SPV_REFLECT_FORMAT_R16_UINT:
			break;
		case SPV_REFLECT_FORMAT_R16_SINT:
			break;
		case SPV_REFLECT_FORMAT_R16_SFLOAT:
			break;
		case SPV_REFLECT_FORMAT_R16G16_UINT:
			break;
		case SPV_REFLECT_FORMAT_R16G16_SINT:
			break;
		case SPV_REFLECT_FORMAT_R16G16_SFLOAT:
			break;
		case SPV_REFLECT_FORMAT_R16G16B16_UINT:
			break;
		case SPV_REFLECT_FORMAT_R16G16B16_SINT:
			break;
		case SPV_REFLECT_FORMAT_R16G16B16_SFLOAT:
			break;
		case SPV_REFLECT_FORMAT_R16G16B16A16_UINT:
			break;
		case SPV_REFLECT_FORMAT_R16G16B16A16_SINT:
			break;
		case SPV_REFLECT_FORMAT_R16G16B16A16_SFLOAT:
			break;
		case SPV_REFLECT_FORMAT_R32_UINT:
			break;
		case SPV_REFLECT_FORMAT_R32_SINT:
			break;
		case SPV_REFLECT_FORMAT_R32_SFLOAT:
			return VertexAttributeType::FLOAT;
		case SPV_REFLECT_FORMAT_R32G32_UINT:
			break;
		case SPV_REFLECT_FORMAT_R32G32_SINT:
			break;
		case SPV_REFLECT_FORMAT_R32G32_SFLOAT:
			return VertexAttributeType::FLOAT2;
		case SPV_REFLECT_FORMAT_R32G32B32_UINT:
			break;
		case SPV_REFLECT_FORMAT_R32G32B32_SINT:
			break;
		case SPV_REFLECT_FORMAT_R32G32B32_SFLOAT:
			return VertexAttributeType::FLOAT3;
		case SPV_REFLECT_FORMAT_R32G32B32A32_UINT:
			break;
		case SPV_REFLECT_FORMAT_R32G32B32A32_SINT:
			break;
		case SPV_REFLECT_FORMAT_R32G32B32A32_SFLOAT:
			return VertexAttributeType::FLOAT4;
		case SPV_REFLECT_FORMAT_R64_UINT:
			break;
		case SPV_REFLECT_FORMAT_R64_SINT:
			break;
		case SPV_REFLECT_FORMAT_R64_SFLOAT:
			break;
		case SPV_REFLECT_FORMAT_R64G64_UINT:
			break;
		case SPV_REFLECT_FORMAT_R64G64_SINT:
			break;
		case SPV_REFLECT_FORMAT_R64G64_SFLOAT:
			break;
		case SPV_REFLECT_FORMAT_R64G64B64_UINT:
			break;
		case SPV_REFLECT_FORMAT_R64G64B64_SINT:
			break;
		case SPV_REFLECT_FORMAT_R64G64B64_SFLOAT:
			break;
		case SPV_REFLECT_FORMAT_R64G64B64A64_UINT:
			break;
		case SPV_REFLECT_FORMAT_R64G64B64A64_SINT:
			break;
		case SPV_REFLECT_FORMAT_R64G64B64A64_SFLOAT:
			break;
		}

		HYRO_LOG_CORE_ERROR("Failed to convert spv reflect format to Vertex Attribute Type");
		return VertexAttributeType::NONE;
	}

	int OpenGLShader::GetUniformLocation(const std::string& name) const
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		int location = glGetUniformLocation(m_Program, name.c_str());
		m_UniformLocationCache[name] = location;

		if (!CheckLocation(location))
		{
			HYRO_LOG_CORE_ERROR("Failed to get uniform location for: {}", name);
		}
		return location;
	}

	bool OpenGLShader::CheckLocation(int location) const
	{
		if (location == -1) {
			return false;
		}
		return true;
	}

	std::string OpenGLShader::ReadShaderFromFile(const std::string& filePath)
	{
		std::ifstream file(filePath);

		if (!file.is_open()) {
			std::cerr << "Failed to open File: " << filePath << std::endl;
			return "";
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

}
