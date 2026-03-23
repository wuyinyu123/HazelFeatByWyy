#pragma once
#include "Hazel/Renderer/Texture.h"

namespace Hazel
{
	class OpenGLTexture : public Texture2D
	{
	public:
		OpenGLTexture(const std::string& path);
		virtual ~OpenGLTexture();

	public:
		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;

		virtual void Bind(uint32_t slot) const override;

	private:
		std::string mPath;
		uint32_t mWidth, mHeight;
		uint32_t mRendererID;
	};
}