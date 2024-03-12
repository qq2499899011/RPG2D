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

// OpenGLTexture is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
namespace RPG2D {
	class OpenGLTexture : public Texture2D
	{
	private:
		// holds the ID of the texture object, used for all texture operations to reference to this particular texture
		uint32_t ID;
		// texture image dimensions
		uint32_t Width, Height; // width and height of loaded image in pixels
		// texture Format
		uint32_t Internal_Format; // format of texture object
		uint32_t Image_Format; // format of loaded image
		// texture configuration
		uint32_t Wrap_S; // wrapping mode on S axis
		uint32_t Wrap_T; // wrapping mode on T axis
		uint32_t Filter_Min; // filtering mode if texture pixels < screen pixels
		uint32_t Filter_Max; // filtering mode if texture pixels > screen pixels
		std::string name;//Ãû³Æ¡£
	public:
		// constructor (sets default texture modes)
		OpenGLTexture(std::string& name);
		// generates texture from image data
		virtual void Generate(uint32_t width, uint32_t height, unsigned char* data) override;
		// binds the texture as the current active GL_TEXTURE_2D texture object
		virtual void Bind(const uint32_t = 0) override;
		virtual void Unbind(const uint32_t = 0) override;
		std::string& GetName() const override;
		virtual void SetInternalFormat(const uint32_t) override;
		virtual void SetImageFormat(const uint32_t) override;
		virtual uint32_t GetID() const override;
	};
}

#endif