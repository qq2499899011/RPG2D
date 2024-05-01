#include "RPG2Dpch.h"
#include "Entity.h"

namespace RPG2D {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{

	}
}