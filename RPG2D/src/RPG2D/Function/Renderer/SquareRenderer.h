#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture2D.h"
#include "VertexArray.h"
#include "Shader.h"

namespace RPG2D {
	class SquareRenderer
	{
	public:
		// Constructor (inits shaders/shapes)
		SquareRenderer(Ref<Shader> shader);
		// Destructor
		~SquareRenderer();
		// Renders a defined quad textured with given sprite
		void DrawSquare(glm::vec3 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec4 color = glm::vec4(1.0f));
		//»æÖÆÁ£×Ó
	private:
		//shader
		Ref<Shader> shader;
		//VAO
		Ref<VertexArray> quadVAO;
		// Initializes and configures the quad's buffer and vertex attributes
		void initRenderData();
	};
}