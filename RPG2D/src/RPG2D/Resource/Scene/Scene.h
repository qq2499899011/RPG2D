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
		//��ȡentt
		Ref<entt::registry> GetRegistry();
		//ʵʱ����
		void OnRuntimeStart();
		void OnRuntimeStop();
		//update����
		void OnUpdateRuntime(Timestep ts);

		//���ӿڴ�С�����仯ʱ
		void OnViewportResize(uint32_t width, uint32_t height);

		bool IsRunning() const { return m_IsRunning; }
		bool IsPaused() const { return m_IsPaused; }

		void SetPaused(bool paused) { m_IsPaused = paused; }

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

		//������ģ�⿪ʼ����ֹͣʱ
		void OnPhysics2DStart();
		void OnPhysics2DStop();
	private:
		//ʵ���������
		entt::registry m_Registry;
		//�����Ӵ��ṹ
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		//��������
		bool m_IsRunning = false;
		//��ͣ��
		bool m_IsPaused = false;
		//TODO:ɾ��
		//���Լ�����
		int m_StepFrames = 0;
		//box2D����
		b2World* m_PhysicsWorld = nullptr;
		//UUID��entity��Ӧ��ϵ
		//ԭ�򣺵�����г�����ʵ�巢��λ�õȱ仯��ԭ��
		std::unordered_map<UID, entt::entity> m_EntityMap;
		friend class Entity;
	};

}
