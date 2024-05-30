/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "RPG2Dpch.h"
#include "SquareRenderer.h"
#include <glad/glad.h>
#include "RenderCommand.h"
#include "Buffer.h"
namespace RPG2D {
	SquareRenderer::SquareRenderer(Ref<Shader> shader)
	{
		this->shader = shader;
		this->initRenderData();
	}

	SquareRenderer::~SquareRenderer()
	{

	}

	//给出texture，transform直接进行渲染即可。
	void SquareRenderer::DrawSquare(glm::vec3 position, glm::vec2 size, float rotate, glm::vec4 color)
	{
		this->shader->Bind();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
		model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back
		//缩放
		model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale
		//设置model到shader
		this->shader->SetMat4("model", model);
		//设置渲染颜色
		this->shader->SetFloat4("spriteColor", color);
		RenderCommand::DrawArray(this->quadVAO, 6);
		this->quadVAO->Unbind();
	}
	//初始化渲染数据
	void SquareRenderer::initRenderData()
	{
		//生成VBO数据
		unsigned int VBO;
		float vertices[] = {
			// pos      
			0.0f, 1.0f, 
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f, 
		};
		//生成VAO
		quadVAO = VertexArray::Create();
		//生成VBO；
		Ref<VertexBuffer> quadVBO = VertexBuffer::Create(vertices, sizeof(vertices));
		quadVAO->Bind();
		//设置相应的layout,只有一个位置信息
		BufferLayout layout = { {ShaderDataType::Float2,"pos"} };
		quadVBO->SetLayout(layout);
		quadVAO->AddVertexBuffer(quadVBO);
	}
}