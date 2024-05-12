#pragma once
#include "RPG2D/Function/Animation/AnimationSystem.h"
#include "RPG2D/Function/Input/InputSystem.h"
#include "RPG2D/Function/Physics/Physics2D.h"
#include "RPG2D/Function/Renderer/RendererManager.h"
#include "RPG2D/Function/Script/ScriptSystem.h"
#include "RPG2D/Resource/AssetManager/AssetManager.h"
#include "RPG2D/Resource/Scene/SceneManager.h"
#include "RPG2D/Events/EventSystem.h"
/// <summary>
/// 管理所有的system和manager
/// </summary>
namespace RPG2D {
	class GlobalContext
	{
	public:
		GlobalContext();
		Ref<AnimationSystem> m_AnimationSystem;
		Ref<ScriptSystem> m_ScriptSystem;
		Ref<PhysicsSystem> m_PhysicsSystem;
		Ref<SceneManager> m_SceneManager;
		Ref<AssetManager> m_AssetManager;
		Ref<RendererManager> m_RendererManager;
		Ref<EventSystem> m_EventSystem;
		//要把常用的操作进行包装。
		static Ref<GlobalContext> GetInstance();
		static void Create();
		void Init();
		void Update(Timestep ts);
	private:
		static Ref<GlobalContext> Instance;
	};
}

