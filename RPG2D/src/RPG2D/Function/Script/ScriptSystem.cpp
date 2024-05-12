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
		//对于每一个实体，遍历所有具有NattiveComponent组件的实体，如果没有实例化就先实例化。
		//对于已经具有实例化的实体，调用update函数。
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		m_Registry->view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
		{
			//如果实例=null
			if (!nsc.Instance)
			{
				//实例化
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->m_Entity = Entity{ entity, GlobalContext::GetInstance()->m_SceneManager->GetSceneActive().get()};
				nsc.Instance->OnCreate();
			}
			//调用实例的OnUpdate函数
			nsc.Instance->OnUpdate(ts);
		});
	}
	void ScriptSystem::Init()
	{
		//初始化
	}
}
