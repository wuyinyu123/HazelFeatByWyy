#pragma once
#include "Renderer.h"


namespace Hazel
{
	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,
		Mat3, Mat4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:  return 4;
			case ShaderDataType::Float2: return 8;
			case ShaderDataType::Float3: return 12;
			case ShaderDataType::Float4: return 16;
			case ShaderDataType::Int:    return 4;
			case ShaderDataType::Int2:   return 8;
			case ShaderDataType::Int3:   return 12;
			case ShaderDataType::Int4:   return 16;
			case ShaderDataType::Mat3:   return 4 * 3 * 3;
			case ShaderDataType::Mat4:   return 4 * 4 * 4;
			case ShaderDataType::Bool:   return 1;
		}

		HZ_CORE_ASSERT(false, "Unknwon ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		uint32_t Offset;
		uint32_t Size;
		ShaderDataType Type;
		bool Normalized;

		BufferElement() = default;
		BufferElement(std::string name, ShaderDataType type) : Name(name),
			Offset(0), Size(ShaderDataTypeSize(type)), Type(type), Normalized(false)
		{

		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:  return 1;
				case ShaderDataType::Float2: return 2;
				case ShaderDataType::Float3: return 3;
				case ShaderDataType::Float4: return 4;
				case ShaderDataType::Int:    return 1;
				case ShaderDataType::Int2:   return 2;
				case ShaderDataType::Int3:   return 3;
				case ShaderDataType::Int4:   return 4;
				case ShaderDataType::Mat3:   return 3 * 3;
				case ShaderDataType::Mat4:   return 4 * 4;
				case ShaderDataType::Bool:   return 1;
			}

			HZ_CORE_ASSERT(false, "Unknwon DataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& element) :
			mElements(element)
		{
			CalculateOffsetAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() const
		{
			return mElements;
		}

	public:
		std::vector<BufferElement>::iterator begin()
		{
			return mElements.begin();
		}
		
		std::vector<BufferElement>::iterator end()
		{
			return mElements.end();
		}
		
		std::vector<BufferElement>::const_iterator begin() const
		{
			return mElements.begin();
		}
		
		std::vector<BufferElement>::const_iterator end() const
		{
			return mElements.end();
		}

		uint32_t GetStride() const
		{
			return mStride;
		}

	private:
		std::vector<BufferElement> mElements;
		uint32_t mStride;

		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			mStride = 0;

			for (auto& element : mElements)
			{
				element.Offset = offset;
				offset += element.Size;
				mStride += element.Size;
			}
		}
	};


	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() { }
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};


	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer()
		{

		}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
		virtual uint32_t GetCount() const = 0;
		
	};
}