/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "RPG2Dpch.h"
#include "SpriteRenderer.h"
#include "RenderCommand.h"
#include "Buffer.h"
namespace RPG2D {
	SpriteRenderer::SpriteRenderer(Ref<Shader> shader)
	{
		this->shader = shader;
		this->initRenderData();
	}

	SpriteRenderer::~SpriteRenderer()
	{

	}

	//����texture��transformֱ�ӽ�����Ⱦ���ɡ�
	void SpriteRenderer::DrawSprite(Ref<Texture2D> texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
	{
		//ʹ��shader
		this->shader->Bind();
		//����modelת������
		glm::mat4 model = glm::mat4(1.0f);
		//��֤ͼƬ����λ��ԭ�㣬Ȼ�����ƶ������ص�ԭ����λ�á�
		//λ��
		model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
		//��ת
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
		model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back
		//����
		model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale
		//����model��shader
		this->shader->SetMat4("model", model);
		//������Ⱦ��ɫ
		this->shader->SetFloat3("spriteColor", color);
		//������
		//glActiveTexture(GL_TEXTURE0);
		texture->Bind();
		//������Ⱦ����
		RenderCommand::DrawLines(this->quadVAO, 6);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		this->quadVAO->Unbind();
	}
	//��ʼ����Ⱦ����
	void SpriteRenderer::initRenderData()
	{
		//����VBO����
		unsigned int VBO;
		float vertices[] = { 
			// pos      // tex
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};
		//����VAO
		quadVAO = VertexArray::Create();
		//����VBO��
		Ref<VertexBuffer> quadVBO = VertexBuffer::Create(vertices,sizeof(vertices));
		quadVAO->Bind();
		//������Ӧ��layout
		//ֻ������һ��layout���С�����vec4���룬ǰ��������Ϊ���λ�ã�����������Ϊ���������Ӧ��ϵ��
		BufferLayout layout = { {ShaderDataType::Float4,"pos_and_tex"} };
		quadVBO->SetLayout(layout);
		quadVAO->AddVertexBuffer(quadVBO);
	}
}