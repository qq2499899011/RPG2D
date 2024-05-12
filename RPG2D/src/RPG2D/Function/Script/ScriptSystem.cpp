#include "RPG2Dpch.h"
#include "ScriptSystem.h"
#include "RPG2D/Resource/ResType/Components.h"
#include "RPG2D/Resource/ResType/Entity.h"
#include "RPG2D/Resource/ResType/ScriptableEntity.h"
namespace RPG2D {
	ScriptSystem::ScriptSystem()
	{
		//
	}
	void ScriptSystem::Update(Timestep ts)
	{
		//����ÿһ��ʵ�壬�������о���NattiveComponent�����ʵ�壬���û��ʵ��������ʵ������
		//�����Ѿ�����ʵ������ʵ�壬����update������
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		m_Registry->view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
		{
			//���ʵ��=null
			if (!nsc.Instance)
			{
				//ʵ����
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->m_Entity = Entity{ entity, GlobalContext::GetInstance()->m_SceneManager->GetSceneActive().get()};
				nsc.Instance->OnCreate();
			}
			//����ʵ����OnUpdate����
			nsc.Instance->OnUpdate(ts);
		});
	}
	void ScriptSystem::Init()
	{
		//��ʼ��
	}
}
