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
		//��մݻ�ʵ��
		void ClearDestroyList();
		//��������б��е�ʵ��
		void DestroyEntityInList();
		//�Ƴ�ʵ�壬����ʵ���б��С�
		void RemoveEntity(Entity entity);
		//����ʵ��
		Entity DuplicateEntity(Entity entity);
		//ͨ�����ֲ���ʵ��
		Entity FindEntityByName(std::string_view name);
		//ͨ��UID��ȡʵ��
		Entity GetEntityByUID(UID uid);
		//�������
		void DisableEntity(Entity entity);
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
		T& GetComponentWithName(const std::string& name)
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
		b2World* GetPhysicsWorld() { return m_PhysicsWorld; }
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
		//�ݻ�ʵ��
		void DestroyEntity(Entity entity);
	private:
		//ʵ���������
		entt::registry m_Registry;
		//box2D����
		b2World* m_PhysicsWorld = nullptr;
		//UID��entity��Ӧ��ϵ��ʵ��ͨ��UID��ʵ����в���
		std::unordered_map<UID, entt::entity> m_EntityMap;
		//�ݴ�����
		std::vector<Entity> m_EntityDestroyList;
		//�������ơ�
		std::string name;
		friend class Entity;
	};

}
