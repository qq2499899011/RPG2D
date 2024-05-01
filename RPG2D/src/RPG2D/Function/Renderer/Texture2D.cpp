#include "RPG2Dpch.h"
#include "Texture2D.h"
#include "RendererManager.h"
#include "Platform/OpenGL/OpenGLTexture.h"
namespace RPG2D {
	Ref<Texture2D> Texture2D::Create(std::string& name)
	{
		switch (RendererManager::GetAPI())
		{
			case RendererAPI::API::None:    RPG2D_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture>(name);
		}

		RPG2D_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
