#include "hzpch.h"
#include "Shader.h"

#include "glad/glad.h"
#include<gtc/type_ptr.hpp>

namespace Hazel
{
	//创建着色器程序
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) : mRendererID(0)
	{
		//创建顶点着色器
		unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		const char* vertex = vertexSrc.c_str();
		glShaderSource(vertexShaderID, 1, &vertex, nullptr);
		glCompileShader(vertexShaderID);

		// 检查顶点着色器编译错误
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShaderID, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//创建片段着色器
		unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		const char* fragment = fragmentSrc.c_str();
		glShaderSource(fragmentShaderID, 1, &fragment, nullptr);
		glCompileShader(fragmentShaderID);

		// 检查片段着色器编译错误
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShaderID, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		mRendererID = glCreateProgram();
		unsigned int program = mRendererID;
		glAttachShader(program, vertexShaderID);
		glAttachShader(program, fragmentShaderID);
		glLinkProgram(program);

		// 检查链接错误
		glGetProgramiv(mRendererID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(mRendererID, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glValidateProgram(program);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	Shader::~Shader()
	{
		glDeleteProgram(mRendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(mRendererID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}


