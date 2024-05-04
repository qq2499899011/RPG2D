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
	}

	void GlobalContext::Init()
	{
		//初始化
	}
	void GlobalContext::Update(Timestep ts)
	{
		//调用所有系统的更新函数
		//物理->脚本->动画->渲染->UI,对场景进行处理。
		m_PhysicsSystem->Update(ts);
		m_ScriptSystem->Update(ts);
		m_AnimationSystem->Update(ts);
		m_RendererManager->Update(ts);
		//TODO：还没有对UI进行封装
	}
}
