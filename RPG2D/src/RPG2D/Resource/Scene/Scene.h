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
		//创建实体
		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUID(UID uid, const std::string& name = std::string());
		//清空摧毁实体
		void ClearDestroyList();
		//清空所有列表中的实体
		void DestroyEntityInList();
		//移除实体，加入实体列表中。
		void RemoveEntity(Entity entity);
		//复制实体
		Entity DuplicateEntity(Entity entity);
		//通过名字查找实体
		Entity FindEntityByName(std::string_view name);
		//通过UID获取实体
		Entity GetEntityByUID(UID uid);
		//禁用组件
		void DisableEntity(Entity entity);
		//增加组件
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
		//获取entt
		entt::registry* GetRegistry();
		//设置物理世界
		void SetPhysicsWorld(b2World*);
		b2World* GetPhysicsWorld() { return m_PhysicsWorld; }
		//获取所有具有此类compoents的实体
		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}
	private:
		//加入组件时
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);	
		//摧毁实体
		void DestroyEntity(Entity entity);
	private:
		//实体组件集合
		entt::registry m_Registry;
		//box2D物理
		b2World* m_PhysicsWorld = nullptr;
		//UID与entity对应关系，实现通过UID对实体进行查找
		std::unordered_map<UID, entt::entity> m_EntityMap;
		//暂存物体
		std::vector<Entity> m_EntityDestroyList;
		//场景名称。
		std::string name;
		friend class Entity;
	};

}
