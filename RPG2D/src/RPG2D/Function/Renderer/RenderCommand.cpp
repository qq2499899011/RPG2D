#include "RPG2Dpch.h"
#include "RPG2D/Function/Renderer/RenderCommand.h"

namespace RPG2D {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}