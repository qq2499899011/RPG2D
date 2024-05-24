#pragma once
/// <summary>
/// 加载texture
/// </summary>
namespace RPG2D {

	class Texture2D
	{
	public:
		virtual	~Texture2D() = default;
		virtual void Generate(uint32_t width, uint32_t height, unsigned char* data) = 0;
		virtual void Bind(const uint32_t = 0) = 0;
		virtual void Unbind(const uint32_t = 0) = 0;
		virtual std::string GetName()const = 0;
		virtual uint32_t GetWidth()const = 0;
		virtual uint32_t GetHeight()const = 0;
		virtual uint32_t GetSubWidth()const = 0;
		virtual uint32_t GetSubHeight()const = 0;
		virtual void SetSubWidth(const uint32_t) = 0;
		virtual void SetSubHeight(const uint32_t) = 0;
		virtual void SetSubWH(const uint32_t,const uint32_t) = 0;
		virtual void SetInternalFormat(const uint32_t) = 0;
		virtual void SetImageFormat(const uint32_t) = 0;
		virtual uint32_t GetID()const = 0;
		//输入xy子纹理序号，获取相应的坐标。
		virtual glm::vec4 GetSubTextureCoords(int, int) = 0;
		static Ref<Texture2D> Create(std::string& name);
	};
}

