#include "hzpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>
#include<gtc/type_ptr.hpp>

namespace Hazel
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		if (type == "fragment" || type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}

		HZ_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	//使用文件创建着色器程序
	OpenGLShader::OpenGLShader(const std::string& path)
	{
		std::string source = ReadFile(path);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		//从文件路径截取出着色器名字
		auto lastSlash = path.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = path.rfind(".");
		auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
		mName = path.substr(lastSlash, count);
	}

	//使用字符串创建着色器程序
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) : mRendererID(0), mName(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(mRendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(mRendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	const std::string OpenGLShader::GetName() const
	{
		return mName;
	}

	
	void OpenGLShader::UploadUniformInt(const std::string& name, const int value) const
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());

		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, const float value) const
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());

		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& vec2) const
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());

		glUniform2f(location, vec2.x, vec2.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& vec3) const
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());

		glUniform3f(location, vec3.x, vec3.y, vec3.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& vec4) const
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());

		glUniform4f(location, vec4.x, vec4.y, vec4.z, vec4.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());

		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
	{
		GLint location = glGetUniformLocation(mRendererID, name.c_str());

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(const std::string& path)
	{
		std::string result;
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			HZ_CORE_ERROR("Could not open file '{0}'", path);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			HZ_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");
			
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] =
				source.substr(nextLinePos,
					pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSource)
	{
		mRendererID = glCreateProgram();
		unsigned int program = mRendererID;

		HZ_CORE_ASSERT(shaderSource.size() <= 2, "shader must be <= 2 now");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;

		//开始编译
		for (auto& kv : shaderSource)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			//创建着色器
			GLuint shader = glCreateShader(type);

			const char* vertex = source.c_str();
			glShaderSource(shader, 1, &vertex, nullptr);
			glCompileShader(shader);

			// 检查着色器编译错误
			int success;
			char infoLog[512];
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 512, nullptr, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		glLinkProgram(program);

		// 检查链接错误
		int success;
		char infoLog[512];
		glGetProgramiv(mRendererID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(mRendererID, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glValidateProgram(program);

		for (auto id : glShaderIDs)
		{
			glDeleteShader(id);
		}
	}
}