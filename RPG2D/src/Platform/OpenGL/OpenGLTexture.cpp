/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "RPG2Dpch.h"
#include <iostream>

#include "OpenGLTexture.h"

namespace RPG2D {
	OpenGLTexture::OpenGLTexture(std::string& name)
		: Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
	{
		this->name = name;
		glCreateTextures(GL_TEXTURE_2D , 1 , &this->ID);
	}

	void OpenGLTexture::Generate(uint32_t width, uint32_t height, unsigned char* data)
	{
		this->Width = width;
		this->Height = height;
		// create Texture
		glBindTexture(GL_TEXTURE_2D, this->ID);
		glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
		// set Texture wrap and filter modes
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
		// unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void OpenGLTexture::Bind(const uint32_t slot = 0)
	{
		glBindTextureUnit(slot, this->ID);
	}
	/// <summary>
	/// 根据槽解绑，基本用不到，因为直接覆盖就行，不需要解绑
	/// </summary>
	/// <param name="slot"></param>
	void OpenGLTexture::Unbind(const uint32_t slot = 0)
	{
		glBindTextureUnit(slot, 0);
	}
	std::string& OpenGLTexture::GetName() const
	{
		// TODO: 在此处插入 return 语句
	}
	void OpenGLTexture::SetInternalFormat(const uint32_t format)
	{
		this->Internal_Format = format;
	}
	void OpenGLTexture::SetImageFormat(const uint32_t format)
	{
		this->Image_Format = format;
	}
	uint32_t OpenGLTexture::GetID() const
	{
		return ID;
	}
}
