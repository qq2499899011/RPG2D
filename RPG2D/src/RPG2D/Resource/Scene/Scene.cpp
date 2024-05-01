#include "RPG2Dpch.h"
#include "Scene.h"
#include "Entity.h"

#include "Components.h"
#include "ScriptableEntity.h"
#include "RPG2D/Function/Physics/Physics2D.h"

#include <glm/glm.hpp>

#include "Entity.h"
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
	}

	Scene::~Scene()
	{
		delete m_PhysicsWorld;
	}

	//�������
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
		//ֱ������һ��UID
		return CreateEntityWithUID(id, name);
	}

	//Ĭ�ϴ���IDComponent������UID����Trasform��TagComponent���������ƣ�
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

	//���٣�ֱ�Ӵ�Map��ɾ�������Ҵݻ�entity
	void Scene::DestroyEntity(Entity entity)
	{
		m_EntityMap.erase(entity.GetUID());
		m_Registry.destroy(entity);
	}

	//״̬ ���� �ű�
	void Scene::OnRuntimeStart()
	{
		//����Ϊ��
		m_IsRunning = true;
		//����ģ�⿪ʼ
		OnPhysics2DStart();

		//֪ͨ�ű�ϵͳ��ʼ
		/*
		{
			//����Onstart
			ScriptEngine::OnRuntimeStart(this);
			// Instantiate all script entities

			//�������нű������ʵ����.
			auto view = m_Registry.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { e, this };
				ScriptEngine::OnCreateEntity(entity);
			}
		}
		*/
	}

	//����OnStopѡ��
	void Scene::OnRuntimeStop()
	{
		m_IsRunning = false;
		OnPhysics2DStop();
		//updateֹͣScriptEngine::OnRuntimeStop();
	}

	//ʵʱ����update ���߼�(�ű�������)����Ⱦ���и���
	void Scene::OnUpdateRuntime(Timestep ts)
	{
		//���������ͣ״̬����Լ�������0ʱ�����г����ĸ��²�����
		if (!m_IsPaused || m_StepFrames-- > 0)
		{
			// Update scripts
			//���½ű�
			/*
			{
				//�������о���ScriptComponent�����ʵ�壬����OnUpdateEntity������
				auto view = m_Registry.view<ScriptComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					ScriptEngine::OnUpdateEntity(entity, ts);
				}
				//
				m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
					{
						// TODO: Move to Scene::OnScenePlay
						if (!nsc.Instance)
						{
							nsc.Instance = nsc.InstantiateScript();
							nsc.Instance->m_Entity = Entity{ entity, this };
							nsc.Instance->OnCreate();
						}

						nsc.Instance->OnUpdate(ts);
					});
			}
			*/

		}
		/*
		// Render 2D
		//�ҵ������
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera)
		{
			//���������
			Renderer2D::BeginScene(*mainCamera, cameraTransform);
			//���ƾ���
			// Draw sprites
			{
				auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				}
			}
			//����ԲȦ
			// Draw circles
			{
				auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
				for (auto entity : view)
				{
					auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

					Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
				}
			}
			//�����ı�
			// Draw text
			{
				auto view = m_Registry.view<TransformComponent, TextComponent>();
				for (auto entity : view)
				{
					auto [transform, text] = view.get<TransformComponent, TextComponent>(entity);

					Renderer2D::DrawString(text.TextString, transform.GetTransform(), text, (int)entity);
				}
			}

			Renderer2D::EndScene();
		}

		*/

	}

	//���Ӵ���С�����仯ʱ
	//TODO:û�����������������á�
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		if (m_ViewportWidth == width && m_ViewportHeight == height)
			return;

		//����viewport
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		//�ҵ�������������Ȼ������viewport
		/*
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
		*/
	}

	//ʵ�帴��,��ʵҲ�൱��Ԥ����
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

	Ref<entt::registry> Scene::GetRegistry()
	{
		return CreateRef<entt::registry>(m_Registry);
	}

	//��������׼��,�����д������������������塢��ײ�У�����Ϣ¼�롣
	void Scene::OnPhysics2DStart()
	{
		//�����������ٶȷ���
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });
		//�ҵ����к��и��������ʵ��
		auto view = m_Registry.view<Rigidbody2DComponent>();
		//����ʵ�壬����ÿ������ʵ�壬��������box2d�еĶ�Ӧbody��
		for (auto e : view)
		{
			//��������ʵ���볡����ϵ
			Entity entity = { e, this };
			//��ȡtransform�͸������
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = Utils::Rigidbody2DTypeToBox2DBody(rb2d.Type);
			bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
			bodyDef.angle = transform.Rotation.z;

			//����һ��b2Body
			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;


			if(entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y, b2Vec2(bc2d.Offset.x, bc2d.Offset.y), 0.0f);
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape circleShape;
				circleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
				circleShape.m_radius = transform.Scale.x * cc2d.Radius;
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnPhysics2DStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	//��Ⱦ���� ��������󣬷ֱ���Ⱦ���顢ԲȦ���ı�

	//�������ʱ����
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

	/*
	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}
	*/


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

}