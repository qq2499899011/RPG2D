/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#pragma once
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RPG2D/Function/Particle/ParticleEmitter.h"

namespace RPG2D {
class SpriteRenderer
{
	public:
		// Constructor (inits shaders/shapes)
		SpriteRenderer(Ref<Shader> shader);
		// Destructor
		~SpriteRenderer();
		// Renders a defined quad textured with given sprite
		void DrawSprite(Ref<Texture2D> texture, glm::vec3 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,bool mirror =false, glm::vec3 color = glm::vec3(1.0f),glm::ivec2 Index = glm::ivec2(0));
		//»æÖÆÁ£×Ó
		void DrawParticle(Ref<ParticleEmitter>,glm::vec3 pos,glm::vec2 size = glm::uvec2(1.0f,1.0f),float rotate = 0.0f);
	private:
		//shader
		Ref<Shader> shader; 
		//VAO
		Ref<VertexArray> quadVAO;
		// Initializes and configures the quad's buffer and vertex attributes
		void initRenderData();
	};
}
#endif