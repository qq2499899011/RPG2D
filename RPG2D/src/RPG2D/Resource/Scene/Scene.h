#pragma once

#include "RPG2D/Core/Timestep.h"
#include "RPG2D/Core/UID.h"
#include "entt.hpp"
class b2World;
namespace RPG2D {

	class Entity;

	class Scene
	{
	public:
		Scene();
		Scene(const std::string& name);
		~Scene();
		std::string& GetName() { return name; }
		//����ʵ��
		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUID(UID uid, const std::string& name = std::string());
		//�ݻ�ʵ��
		void DestroyEntity(Entity entity);
		//����ʵ��
		Entity DuplicateEntity(Entity entity);
		//ͨ�����ֲ���ʵ��
		Entity FindEntityByName(std::string_view name);
		//ͨ��UID��ȡʵ��
		Entity GetEntityByUID(UID uid);
		//�������
		template<typename T>
		bool AddComponentWithName(const std::string& name, T& component) {
			Entity entity = FindEntityByName(name);
			entity.AddComponent<T>(component);
			return true;
		}

		template<typename T>
		bool AddComponentWithUID(UID uid, T& component)
		{
			Entity entity = GetEntityByUID(uid);
			entity.AddComponent<T>(component);
			return true;
		}

		template<typename T>
		T& GetComponentByName(const std::string& name)
		{
			Entity entity = FindEntityByName(name);
			return entity.GetComponent<T>();
		}

		template<typename T>
		T& GetComponentWithUID(UID uid)
		{
			Entity entity = GetEntityByUID(uid);
			return entity.GetComponent<T>();
		}
		//��ȡentt
		entt::registry* GetRegistry();
		//������������
		void SetPhysicsWorld(b2World*);
		//��ȡ���о��д���compoents��ʵ��
		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}
	private:
		//�������ʱ
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		//ʵ���������
		entt::registry m_Registry;
		//box2D����
		b2World* m_PhysicsWorld = nullptr;
		//UUID��entity��Ӧ��ϵ
		//ԭ�򣺵�����г�����ʵ�巢��λ�õȱ仯��ԭ��
		std::unordered_map<UID, entt::entity> m_EntityMap;
		std::string name;
		friend class Entity;
	};

}
