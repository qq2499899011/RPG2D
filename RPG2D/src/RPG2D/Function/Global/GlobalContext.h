#pragma once
#include "RPG2D/Function/Animation/AnimationSystem.h"
#include "RPG2D/Function/Input/InputSystem.h"
#include "RPG2D/Function/Physics/Physics2D.h"
#include "RPG2D/Function/Renderer/RendererManager.h"
#include "RPG2D/Resource/AssetManager/AssetManager.h"
#include "RPG2D/Resource/Scene/SceneManager.h"
/// <summary>
/// 管理所有的system和manager
/// </summary>
namespace RPG2D {
	class GlobalContext
	{
	public:
		GlobalContext();
		Ref<AnimationSystem> m_AnimationSystem;
		Ref<PhysicsSystem> m_PhysicsSystem;
		Ref<SceneManager> m_SceneManager;
		Ref<AssetManager> m_AssetManager;
		Ref<RendererManager> m_RendererManager;
		static Ref<GlobalContext> GetInstace();
		static void Create();
		void Init();
	private:
		static Ref<GlobalContext> Instance;
	};
}

