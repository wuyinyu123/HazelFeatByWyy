#pragma once


#include "../../../vendor/glm/glm.hpp"
#include "Hazel/Renderer/Shader.h"

typedef unsigned int GLenum;

namespace Hazel
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& path);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

	public:
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual const std::string GetName() const override;

	public:
		void UploadUniformInt(const std::string& name, const int value) const;

		void UploadUniformFloat(const std::string& name, const float value) const;
		void UploadUniformFloat2(const std::string& name, const glm::vec2& vec2) const;
		void UploadUniformFloat3(const std::string& name, const glm::vec3& vec3) const;
		void UploadUniformFloat4(const std::string& name, const glm::vec4& vec4) const;

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

	private:
		std::string ReadFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSource);

	private:
		uint32_t mRendererID;
		std::string mName;
	};
}
