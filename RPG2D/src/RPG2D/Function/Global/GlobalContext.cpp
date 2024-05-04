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
		//����Instance
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
		//��ʼ��
	}
	void GlobalContext::Update(Timestep ts)
	{
		//��������ϵͳ�ĸ��º���
		//����->�ű�->����->��Ⱦ->UI,�Գ������д���
		m_PhysicsSystem->Update(ts);
		m_ScriptSystem->Update(ts);
		m_AnimationSystem->Update(ts);
		m_RendererManager->Update(ts);
		//TODO����û�ж�UI���з�װ
	}
}
