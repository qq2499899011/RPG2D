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

	//给出texture，transform直接进行渲染即可。
	void SpriteRenderer::DrawSprite(Ref<Texture2D> texture, glm::vec3 position, glm::vec2 size, float rotate,bool mirror, glm::vec3 color,glm::ivec2 index)
	{
		//使用shader
		this->shader->Bind();
		//设置model转化矩阵
		glm::mat4 model = glm::mat4(1.0f);
		//保证图片中心位于原点，然后再移动，最后回到原来的位置。
		//位移
		model = glm::translate(model, glm::vec3(position));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
		//旋转
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
		if(!mirror)model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // 正常
		else model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // 镜像
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back
		//缩放
		model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale
		//设置model到shader
		this->shader->SetMat4("model", model);
		//设置渲染颜色
		this->shader->SetFloat3("spriteColor", color);
		glm::vec4 subc = texture->GetSubTextureCoords(index.x, index.y);
		this->shader->SetFloat4("subTextureCoords", subc);
		//绑定纹理
		//glActiveTexture(GL_TEXTURE0);
		texture->Bind();
		//调用渲染函数
		RenderCommand::DrawArray(this->quadVAO, 6);
		this->quadVAO->Unbind();
	}
	//绘制粒子发射器控制的所有粒子
	void SpriteRenderer::DrawParticle(Ref<ParticleEmitter> emitter,glm::vec3 pos,glm::vec2 size,float rotation)
	{
		//使用shader
		this->shader->Bind();
		//获取材质并绑定
		Ref<Texture2D> texture = emitter->GetTexture();
		texture->Bind();
		//遍历emitter中的所有粒子进行绘制
		for (Particle& particle : emitter->GetParticles()) {
			if (particle.life <= 0.0f)continue;
			//设置model转化矩阵
			glm::mat4 model = glm::mat4(1.0f);
			//位移
			model = glm::translate(model, glm::vec3(pos)+glm::vec3(particle.position,0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
			//旋转
			glm::vec2 pSize = glm::vec2(size.x * particle.size.x, size.y * particle.size.y);
			model = glm::translate(model, glm::vec3(0.5f * pSize.x, 0.5f * pSize.y, 0.0f)); // move origin of rotation to center of quad
			model = glm::rotate(model, glm::radians(rotation+particle.rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // 正常
			model = glm::translate(model, glm::vec3(-0.5f * pSize.x, -0.5f * pSize.y, 0.0f)); // move origin back
			//缩放
			model = glm::scale(model, glm::vec3(pSize, 1.0f)); // last scale
			//设置model到shader
			this->shader->SetMat4("model", model);
			//设置渲染颜色
			this->shader->SetFloat3("spriteColor", particle.color);
			//设置纹理映射
			this->shader->SetFloat4("subTextureCoords", texture->GetSubTextureCoords(0, 0));
			//调用渲染函数
			RenderCommand::DrawArray(this->quadVAO, 6);
		}
		//调用
		this->quadVAO->Unbind();
	}
	//初始化渲染数据
	void SpriteRenderer::initRenderData()
	{
		//生成VBO数据
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

		//生成VAO
		quadVAO = VertexArray::Create();
		//生成VBO；
		Ref<VertexBuffer> quadVBO = VertexBuffer::Create(vertices,sizeof(vertices));
		quadVAO->Bind();
		//设置相应的layout
		//只用生成一个layout就行。按照vec4传入，前两个数据为点的位置，后两个数据为纹理坐标对应关系。
		BufferLayout layout = { {ShaderDataType::Float4,"pos_and_tex"} };
		quadVBO->SetLayout(layout);
		quadVAO->AddVertexBuffer(quadVBO);
	}
}