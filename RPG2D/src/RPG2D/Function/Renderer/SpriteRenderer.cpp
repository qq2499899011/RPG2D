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
#include <glad/glad.h>
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
	void SpriteRenderer::DrawSprite(Ref<Texture2D> texture, glm::vec3 position, glm::vec2 size, float rotate,bool mirror, glm::vec3 color,glm::ivec2 index)
	{
		//ʹ��shader
		this->shader->Bind();
		//����modelת������
		glm::mat4 model = glm::mat4(1.0f);
		//��֤ͼƬ����λ��ԭ�㣬Ȼ�����ƶ������ص�ԭ����λ�á�
		//λ��
		model = glm::translate(model, glm::vec3(position));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
		//��ת
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
		if(!mirror)model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // ����
		else model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // ����
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back
		//����
		model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale
		//����model��shader
		this->shader->SetMat4("model", model);
		//������Ⱦ��ɫ
		this->shader->SetFloat3("spriteColor", color);
		glm::vec4 subc = texture->GetSubTextureCoords(index.x, index.y);
		this->shader->SetFloat4("subTextureCoords", subc);
		//������
		//glActiveTexture(GL_TEXTURE0);
		texture->Bind();
		//������Ⱦ����
		RenderCommand::DrawArray(this->quadVAO, 6);
		this->quadVAO->Unbind();
	}
	//�������ӷ��������Ƶ���������
	void SpriteRenderer::DrawParticle(Ref<ParticleEmitter> emitter,glm::vec3 pos,glm::vec2 size,float rotation)
	{
		//ʹ��shader
		this->shader->Bind();
		//��ȡ���ʲ���
		Ref<Texture2D> texture = emitter->GetTexture();
		texture->Bind();
		//����emitter�е��������ӽ��л���
		for (Particle& particle : emitter->GetParticles()) {
			if (particle.life <= 0.0f)continue;
			//����modelת������
			glm::mat4 model = glm::mat4(1.0f);
			//λ��
			model = glm::translate(model, glm::vec3(pos)+glm::vec3(particle.position,0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
			//��ת
			glm::vec2 pSize = glm::vec2(size.x * particle.size.x, size.y * particle.size.y);
			model = glm::translate(model, glm::vec3(0.5f * pSize.x, 0.5f * pSize.y, 0.0f)); // move origin of rotation to center of quad
			model = glm::rotate(model, glm::radians(rotation+particle.rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // ����
			model = glm::translate(model, glm::vec3(-0.5f * pSize.x, -0.5f * pSize.y, 0.0f)); // move origin back
			//����
			model = glm::scale(model, glm::vec3(pSize, 1.0f)); // last scale
			//����model��shader
			this->shader->SetMat4("model", model);
			//������Ⱦ��ɫ
			this->shader->SetFloat3("spriteColor", particle.color);
			//��������ӳ��
			this->shader->SetFloat4("subTextureCoords", texture->GetSubTextureCoords(0, 0));
			//������Ⱦ����
			RenderCommand::DrawArray(this->quadVAO, 6);
		}
		//����
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