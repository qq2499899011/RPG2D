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

	//给出texture，transform直接进行渲染即可。
	void SpriteRenderer::DrawSprite(Ref<Texture2D> texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 color)
	{
		//使用shader
		this->shader->Bind();
		//设置model转化矩阵
		glm::mat4 model = glm::mat4(1.0f);
		//保证图片中心位于原点，然后再移动，最后回到原来的位置。
		//位移
		model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
		//旋转
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
		model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back
		//缩放
		model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale
		//设置model到shader
		this->shader->SetMat4("model", model);
		//设置渲染颜色
		this->shader->SetFloat3("spriteColor", color);
		//绑定纹理
		//glActiveTexture(GL_TEXTURE0);
		texture->Bind();
		//调用渲染函数
		RenderCommand::DrawLines(this->quadVAO, 6);
		glDrawArrays(GL_TRIANGLES, 0, 6);
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