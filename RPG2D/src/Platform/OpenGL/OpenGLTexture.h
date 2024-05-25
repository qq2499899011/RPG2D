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

//����ID ���� 
namespace RPG2D {
	class OpenGLTexture : public Texture2D
	{
	private:
		// holds the ID of the texture object, used for all texture operations to reference to this particular texture
		uint32_t ID;
		// texture image dimensions
		uint32_t Width, Height; // ͼƬ�����س���,��ȡ�����С��
		uint32_t SubWidth, SubHeight;//��������
		// texture Format
		uint32_t Internal_Format; //�ڲ���ʽ����δ洢����
		uint32_t Image_Format; // ���ظ�ʽ����μ�������
		// texture configuration
		uint32_t Wrap_S; // ˮƽ�ᷴתģʽ
		uint32_t Wrap_T; // ��ֱ�ᷴת
		uint32_t Filter_Min; // ����Сʱ��ȡ�Ĺ���ģʽ
		uint32_t Filter_Max; // ���Ŵ�ʱ��ȡ�Ĺ���ģʽ
		std::string name;//����
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