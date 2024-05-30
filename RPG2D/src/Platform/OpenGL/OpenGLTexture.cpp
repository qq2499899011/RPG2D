#include "RPG2Dpch.h"
#include <iostream>

#include "OpenGLTexture.h"

namespace RPG2D {
	//�����ʼ������
	//Ĭ�ϳ���0,�ڲ���ʽ�ͼ��ظ�ʽRGB������ģʽ�ظ�������ģʽ����
	OpenGLTexture::OpenGLTexture(std::string& name)
		: Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_MIRRORED_REPEAT), Wrap_T(GL_MIRRORED_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
	{
		//��������
		this->name = name;
		//����texture
		glCreateTextures(GL_TEXTURE_2D , 1 , &this->ID);
	}

	//���볤���Լ���Ӧ���������ݣ���������
	void OpenGLTexture::Generate(uint32_t width, uint32_t height, unsigned char* data)
	{
		//���ó���
		this->Width = width;
		this->Height = height;
		//����Ĭ����������
		SubWidth = width;
		SubHeight = height;
		//��������������
		glBindTexture(GL_TEXTURE_2D, this->ID);
		glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
		//���û���ģʽ�͹���ģʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
		//ȡ����
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	//
	void OpenGLTexture::Bind(const uint32_t slot)
	{
		glBindTextureUnit(slot, this->ID);
	}
	/// <summary>
	/// ���ݲ۽�󣬻����ò�������Ϊֱ�Ӹ��Ǿ��У�����Ҫ���
	/// </summary>
	/// <param name="slot"></param>
	void OpenGLTexture::Unbind(const uint32_t slot)
	{
		glBindTextureUnit(slot, 0);
	}
	std::string OpenGLTexture::GetName() const
	{
		// TODO: �ڴ˴����� return ���
		return this->name;
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
	uint32_t OpenGLTexture::GetWidth() const
	{
		return Width;
	}
	uint32_t OpenGLTexture::GetHeight() const
	{
		return Height;
	}
	uint32_t OpenGLTexture::GetSubWidth() const
	{
		return SubWidth;
	}
	uint32_t OpenGLTexture::GetSubHeight() const
	{
		return SubHeight;
	}
	void OpenGLTexture::SetSubWidth(const uint32_t width)
	{
		SubWidth = width;
	}
	void OpenGLTexture::SetSubHeight(const uint32_t height)
	{
		SubHeight = height;
	}
	glm::vec4 OpenGLTexture::GetSubTextureCoords(int xIndex, int yIndex)
	{
		float x0 = (xIndex * SubWidth) / static_cast<float>(Width);
		float y0 = (yIndex * SubHeight) / static_cast<float>(Height);
		float x1 = ((xIndex + 1) * SubWidth) / static_cast<float>(Width);
		float y1 = ((yIndex + 1) * SubHeight) / static_cast<float>(Height);
		return glm::vec4(x0, y0, x1, y1);
	}
	void OpenGLTexture::SetSubWH(const uint32_t w, const uint32_t h)
	{
		SubWidth = w;
		SubHeight = h;
	}
	glm::vec2 OpenGLTexture::GetSize() const
	{
		return glm::vec2(Width,Height);
	}
}
