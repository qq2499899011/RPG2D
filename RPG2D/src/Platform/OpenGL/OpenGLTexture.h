/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef TEXTURE_H
#define TEXTURE_H
#include "RPG2D/Function/Renderer/Texture2D.h"
#include <glad/glad.h>

//储存ID 长宽 
namespace RPG2D {
	class OpenGLTexture : public Texture2D
	{
	private:
		// holds the ID of the texture object, used for all texture operations to reference to this particular texture
		uint32_t ID;
		// texture image dimensions
		uint32_t Width, Height; // 图片的像素长宽,获取本身大小。
		uint32_t SubWidth, SubHeight;//子纹理长宽
		// texture Format
		uint32_t Internal_Format; //内部格式，如何存储数据
		uint32_t Image_Format; // 加载格式，如何加载数据
		// texture configuration
		uint32_t Wrap_S; // 水平轴反转模式
		uint32_t Wrap_T; // 垂直轴反转
		uint32_t Filter_Min; // 被缩小时采取的过滤模式
		uint32_t Filter_Max; // 被放大时采取的过滤模式
		std::string name;//名称
	public:
		// constructor (sets default texture modes)
		OpenGLTexture(std::string& name);
		// generates texture from image data
		virtual void Generate(uint32_t width, uint32_t height, unsigned char* data) override;
		// binds the texture as the current active GL_TEXTURE_2D texture object
		virtual void Bind(const uint32_t = 0) override;
		virtual void Unbind(const uint32_t = 0) override;
		virtual std::string GetName() const override;
		virtual void SetInternalFormat(const uint32_t) override;
		virtual void SetImageFormat(const uint32_t) override;
		virtual uint32_t GetID() const override;
		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;
		virtual uint32_t GetSubWidth() const override;
		virtual uint32_t GetSubHeight() const override;
		virtual void SetSubWidth(const uint32_t) override;
		virtual void SetSubHeight(const uint32_t) override;
		virtual glm::vec4 GetSubTextureCoords(int, int) override;
		virtual void SetSubWH(const uint32_t, const uint32_t) override;
		virtual glm::vec2 GetSize() const override;
	};
}

#endif