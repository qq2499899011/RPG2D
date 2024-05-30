#include "RPG2Dpch.h"
#include "Entity.h"

namespace RPG2D {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{

	}
	glm::vec2 Entity::GetCenter()
	{
		//获取角色中心位置，只有同时拥有Transform和SpriteRenderer才行
		if (HasComponent<TransformComponent>() && HasComponent<SpriteRendererComponent>()) {
			//获取两个组件
			TransformComponent& trans = GetComponent<TransformComponent>();
			SpriteRendererComponent& sprite = GetComponent<SpriteRendererComponent>();
			//计算中心位置
			//0左 0上 加上2/1，其他是减法,所以映射到1 ，-1
			float calX = 1 - 2 * trans.anchor.x;
			float calY = 1 - 2 * trans.anchor.y;
			//获取素材大小
			glm::vec2 size = sprite.Texture->GetSize();
			//计算缩放
			size.x *= trans.Scale.x;
			size.y *= trans.Scale.y;
			//计算最终的位置
			return glm::vec2(trans.Translation.x + calX * size.x / 2, trans.Translation.y + calY * size.y / 2);
		}
		RPG2D_CORE_WARN("Don't have SpriteRendererComponent");
		return glm::vec2(0, 0);
	}
	glm::vec2 Entity::GetPos()
	{
		TransformComponent& trans = GetComponent<TransformComponent>();
		return glm::vec2(trans.Translation.x, trans.Translation.y);
	}
	glm::vec2 Entity::GetScale()
	{
		TransformComponent& trans = GetComponent<TransformComponent>();
		return glm::vec2(trans.Scale.x,trans.Scale.y);
	}
	const std::string& Entity::GetName()
	{
		if (HasComponent<TagComponent>()) {
			return GetComponent<TagComponent>().Tag;
		}
		else {
			RPG2D_CORE_ERROR("Don't have name");
			return "Invalid";
		}
	}
}