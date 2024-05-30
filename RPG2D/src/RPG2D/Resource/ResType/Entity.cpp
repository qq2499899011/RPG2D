#include "RPG2Dpch.h"
#include "Entity.h"

namespace RPG2D {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{

	}
	glm::vec2 Entity::GetCenter()
	{
		//��ȡ��ɫ����λ�ã�ֻ��ͬʱӵ��Transform��SpriteRenderer����
		if (HasComponent<TransformComponent>() && HasComponent<SpriteRendererComponent>()) {
			//��ȡ�������
			TransformComponent& trans = GetComponent<TransformComponent>();
			SpriteRendererComponent& sprite = GetComponent<SpriteRendererComponent>();
			//��������λ��
			//0�� 0�� ����2/1�������Ǽ���,����ӳ�䵽1 ��-1
			float calX = 1 - 2 * trans.anchor.x;
			float calY = 1 - 2 * trans.anchor.y;
			//��ȡ�زĴ�С
			glm::vec2 size = sprite.Texture->GetSize();
			//��������
			size.x *= trans.Scale.x;
			size.y *= trans.Scale.y;
			//�������յ�λ��
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