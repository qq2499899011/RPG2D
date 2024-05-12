#include "RPG2Dpch.h"
#include "Scene.h"
#include "RPG2D/Resource/ResType/Entity.h"
#include "RPG2D/Resource/ResType/Components.h"
#include "RPG2D/Resource/ResType/ScriptableEntity.h"
#include "RPG2D/Function/Physics/Physics2D.h"
#include <glm/glm.hpp>
#include "RPG2D/Core/UID.h"
// Box2D
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"

namespace RPG2D {

	Scene::Scene()
	{
		// 设置默认的过滤条件，排除带有"Disable"标签的实体
		m_Registry.set<entt::exclude_t<Disable>>();
	}

	Scene::Scene(const std::string& name)
	{
		// 设置默认的过滤条件，排除带有"Disable"标签的实体
		m_Registry.set<entt::exclude_t<Disable>>();
		this->name = name;
	}

	Scene::~Scene()
	{
		delete m_PhysicsWorld;
	}

	//复制组件
	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UID, entt::entity>& enttMap)
	{
		([&]()
			{
				auto view = src.view<Component>();
				for (auto srcEntity : view)
				{
					entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);

					auto& srcComponent = src.get<Component>(srcEntity);
					dst.emplace_or_replace<Component>(dstEntity, srcComponent);
				}
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
			{
				if (src.HasComponent<Component>())
					dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}


	Entity Scene::CreateEntity(const std::string& name)
	{
		UID id;
		//直接生成一个UID
		return CreateEntityWithUID(id, name);
	}

	//默认存入IDComponent（储存UID）、Trasform、TagComponent（储存名称）
	Entity Scene::CreateEntityWithUID(UID uid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;
		m_EntityMap[uid] = entity;
		return entity;
	}
	void Scene::ClearDestroyList()
	{
		m_EntityDestroyList.clear();
	}
	void Scene::DestroyEntityInList()
	{
		//删除全部实体
		for (Entity entity : m_EntityDestroyList) {
			DestroyEntity(entity);
		}
		m_EntityDestroyList.clear();
	}
	void Scene::RemoveEntity(Entity entity)
	{
		//先加入列表中
		m_EntityDestroyList.push_back(entity);
		//同时加入物理列表中
		GlobalContext::GetInstance()->m_PhysicsSystem->RemoveEntity(entity);
	}
	//销毁，直接从Map中删除，并且摧毁entity
	void Scene::DestroyEntity(Entity entity)
	{
		m_EntityMap.erase(entity.GetUID());
		m_Registry.destroy(entity);
	}
	//实体复制,其实也相当于预制体
	Entity Scene::DuplicateEntity(Entity entity)
	{
		// Copy name because we're going to modify component data structure
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);
		CopyComponentIfExists(AllComponents{}, newEntity, entity);
		return newEntity;
	}
	Entity Scene::FindEntityByName(std::string_view name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tc = view.get<TagComponent>(entity);
			if (tc.Tag == name)
				return Entity{ entity, this };
		}
		return {};
	}
	Entity Scene::GetEntityByUID(UID uid)
	{
		// TODO(Yan): Maybe should be assert
		if (m_EntityMap.find(uid) != m_EntityMap.end())
			return { m_EntityMap.at(uid), this };

		return {};
	}

	void Scene::DisableEntity(Entity entity)
	{
		m_Registry.assign<Disable>(entity);
	}

	entt::registry* Scene::GetRegistry()
	{
		return &m_Registry;
	}

	void Scene::SetPhysicsWorld(b2World* world)
	{
		m_PhysicsWorld = world;
	}

	//加入组件时调用
	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<Rigidbody2DComponent>(Entity entity, Rigidbody2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
		//实例化
		if (!component.Instance)
		{
			//实例化
			component.Instance = component.InstantiateScript();
			component.Instance->m_Entity = Entity{ entity, GlobalContext::GetInstance()->m_SceneManager->GetSceneActive().get() };
			component.Instance->OnCreate();
		}
	}
	template<>
	void Scene::OnComponentAdded<AnimatiorControllerComponent>(Entity entity, AnimatiorControllerComponent& component)
	{
	}
}
