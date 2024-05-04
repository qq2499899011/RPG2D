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
		//摧毁实体
		void DestroyEntity(Entity entity);
		//复制实体
		Entity DuplicateEntity(Entity entity);
		//通过名字查找实体
		Entity FindEntityByName(std::string_view name);
		//通过UID获取实体
		Entity GetEntityByUID(UID uid);
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
		//获取entt
		entt::registry* GetRegistry();
		//设置物理世界
		void SetPhysicsWorld(b2World*);
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
	private:
		//实体组件集合
		entt::registry m_Registry;
		//box2D物理
		b2World* m_PhysicsWorld = nullptr;
		//UUID与entity对应关系
		//原因：点击运行场景，实体发生位置等变化复原。
		std::unordered_map<UID, entt::entity> m_EntityMap;
		std::string name;
		friend class Entity;
	};

}
