#include "RPG2Dpch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace RPG2D {
	Ref<Shader> Shader::Create(const char* vertexSrc, const char* fragmentSrc,const char* gSrc )
	{
		switch (RendererManager::GetAPI())
		{
			case RendererAPI::API::None:    RPG2D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLShader>(vertexSrc, fragmentSrc,gSrc);
		}

		RPG2D_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}