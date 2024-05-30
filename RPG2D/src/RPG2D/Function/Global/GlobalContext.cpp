#include "RPG2Dpch.h"
#include "GlobalContext.h"
namespace RPG2D {
	Ref<GlobalContext> GlobalContext::Instance = nullptr;
	GlobalContext::GlobalContext()
	{
		//
	}

	Ref<GlobalContext> GlobalContext::GetInstance()
	{
		return Instance;
	}

	void GlobalContext::Create()
	{
		//创建Instance
		Instance = CreateRef<GlobalContext>();
		Instance->m_AnimationSystem = CreateRef<AnimationSystem>();
		Instance->m_ScriptSystem = CreateRef<ScriptSystem>();
		Instance->m_PhysicsSystem = CreateRef<PhysicsSystem>();
		Instance->m_SceneManager = CreateRef<SceneManager>();
		Instance->m_AssetManager = CreateRef<AssetManager>(); 
		Instance->m_RendererManager = CreateRef<RendererManager>(); 
		Instance->m_EventSystem = CreateRef<EventSystem>();
		Instance->m_UISystem = CreateRef<UISystem>();
		Instance->m_ParticleSystem = CreateRef<ParticleSystem>();
		Instance->m_AudioSystem = CreateRef<AudioSystem>();
	}

	void GlobalContext::Init()
	{
		//初始化
		m_UISystem->Init();
		//OnStart
		//m_ScriptSystem->Init();
	}
	void GlobalContext::Update(Timestep ts)
	{
		//调用所有系统的更新函数
		m_PhysicsSystem->DestroyBodies();
		m_SceneManager->GetSceneActive()->DestroyEntityInList();
		//物理->脚本->动画->音频->渲染->粒子->UI,对场景进行处理。
		m_PhysicsSystem->Update(ts);
		//物理世界的物体优先于场景销毁
		m_ScriptSystem->Update(ts);
		m_AudioSystem->Update(ts);
		m_AnimationSystem->Update(ts);
		m_ParticleSystem->Update(ts);
		m_RendererManager->Update(ts);
		m_UISystem->Update(ts);
		//TODO：还没有对UI进行封装
	}
	void GlobalContext::DeInit()
	{
		GlobalContext::GetInstance()->m_AudioSystem->DeInit();
		GlobalContext::GetInstance()->m_ScriptSystem->DeInit();
		GlobalContext::GetInstance()->m_AssetManager->Clear();
		GlobalContext::GetInstance()->m_SceneManager->DeInit();
	}
}
