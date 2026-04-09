#include "pch.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glad/glad.h>

#include "Hyro/Core/Core.h"


#include <fstream>
#include <sstream>

namespace Hyro {

	OpenGLShader::OpenGLShader(const std::string& vertexPath, const std::string& fragPath)
		: m_Program(0)
	{
		std::string vertexSource = readShaderFromFile(vertexPath);
		std::string fragmentSource = readShaderFromFile(fragPath);

		const char* vertexShaderSourceCStr = vertexSource.c_str();
		const char* fragmentShaderSourceCStr = fragmentSource.c_str();
		uint32_t vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSourceCStr);
		uint32_t fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSourceCStr);

		m_Program = glCreateProgram();
		glAttachShader(m_Program, vertexShader);
		glAttachShader(m_Program, fragmentShader);
		glLinkProgram(m_Program);
		checkLinkingErrors();

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_Program);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_Program);
	}

	void OpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetUniformInt(const std::string& name, int value) const
	{
		Bind();
		int location = getUniformLocation(name);
		checkLocation(location);
		glUniform1i(location, value);
	}

	void OpenGLShader::SetUniformFloat(const std::string& name, float value) const
	{
		Bind();
		int location = getUniformLocation(name);
		glUniform1f(location, value);
	}

	void OpenGLShader::SetUnifromBool(const std::string& name, bool value) const
	{
		Bind();
		int location = getUniformLocation(name);
		glUniform1i(location, (int)value);
	}

	void OpenGLShader::setUniformVec3(const std::string& name, const glm::vec3& value) const
	{
		Bind();
		int location = getUniformLocation(name);
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetUniformVec4(const std::string& name, const glm::vec4& value) const
	{
		Bind();
		int location = getUniformLocation(name);
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::setUniformMat4(const std::string& name, const glm::mat4& value) const
	{
		Bind();
		int location = getUniformLocation(name);
		glUniformMatrix4fv(location, 1, false, glm::value_ptr(value));
	}

	int OpenGLShader::getUniformLocation(const std::string& name) const
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		int location = glGetUniformLocation(m_Program, name.c_str());
		m_UniformLocationCache[name] = location;
		return location;
	}

	void OpenGLShader::checkShaderCompilation(uint32_t shader)
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

	void OpenGLShader::checkLinkingErrors()
	{
		int success;
		char infoLog[512];
		glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
			HYRO_LOG_CORE_ERROR("Shader linking failed! \n {}", infoLog);
		}
	}

	void OpenGLShader::checkLocation(int location) const
	{
		if (location == -1) {
			HYRO_LOG_CORE_ERROR("Tried to access invalid Uniform Location");
		}
	}

	uint32_t OpenGLShader::createShader(uint32_t shaderType, const char* shaderSource)
	{
		uint32_t shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &shaderSource, NULL);
		glCompileShader(shader);
		checkShaderCompilation(shader);

		return shader;
	}

	std::string OpenGLShader::readShaderFromFile(const std::string& filePath)
	{
		std::ifstream file(filePath);

		if (!file.is_open()) {
			std::cerr << "Failed to open File: " << filePath << std::endl;
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

}
