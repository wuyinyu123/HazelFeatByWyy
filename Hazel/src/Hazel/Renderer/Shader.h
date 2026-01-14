#pragma once

namespace Hazel
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

	public:
		void Bind() const;
		void Unbind() const;

	private:
		uint32_t mRendererID;
	};
}



