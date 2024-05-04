#include "RPG2Dpch.h"
#include "SceneManager.h"
namespace RPG2D {
	SceneManager::SceneManager()
	{
		//默认从第0号场景开始
		activeSceneNum = 0;
	}

	//设置活跃场景
	Ref<Scene> SceneManager::SetSceneActive(int num)
	{
		activeSceneNum = num;
		//设置后初始化物理场景
		GlobalContext::GetInstance()->m_PhysicsSystem->Init();
		return scenes[num];
	}
	//通过场景名称设置活跃场景
	Ref<Scene> SceneManager::SetSceneActive(const std::string& name) {
		for (size_t i = 0; i < scenes.size(); i++) {
			if ((scenes[i]->GetName()).compare(name) == 0) {
				//比较名称是否一致
				return SetSceneActive(i);
			}
		}
	}

	bool SceneManager::AddScene(Ref<Scene> scene)
	{
		scenes.push_back(scene);
		return true;
	}

	entt::registry* SceneManager::GetRegistry()
	{
		return scenes[activeSceneNum]->GetRegistry();
	}

	Ref<Scene> SceneManager::GetSceneActive()
	{
		return scenes[activeSceneNum];
	}
	
}
