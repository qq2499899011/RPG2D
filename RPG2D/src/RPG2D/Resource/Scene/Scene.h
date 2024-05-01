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
		~Scene();

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
		//获取entt
		Ref<entt::registry> GetRegistry();
		//实时运行
		void OnRuntimeStart();
		void OnRuntimeStop();
		//update更新
		void OnUpdateRuntime(Timestep ts);

		//当视口大小发生变化时
		void OnViewportResize(uint32_t width, uint32_t height);

		bool IsRunning() const { return m_IsRunning; }
		bool IsPaused() const { return m_IsPaused; }

		void SetPaused(bool paused) { m_IsPaused = paused; }

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

		//当物理模拟开始或者停止时
		void OnPhysics2DStart();
		void OnPhysics2DStop();
	private:
		//实体组件集合
		entt::registry m_Registry;
		//场景视窗结构
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		//正在运行
		bool m_IsRunning = false;
		//暂停中
		bool m_IsPaused = false;
		//TODO:删除
		//调试计数器
		int m_StepFrames = 0;
		//box2D物理
		b2World* m_PhysicsWorld = nullptr;
		//UUID与entity对应关系
		//原因：点击运行场景，实体发生位置等变化复原。
		std::unordered_map<UID, entt::entity> m_EntityMap;
		friend class Entity;
	};

}
