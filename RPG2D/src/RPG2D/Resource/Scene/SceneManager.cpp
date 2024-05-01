#include "RPG2Dpch.h"
#include "SceneManager.h"
namespace RPG2D {
	SceneManager::SceneManager()
	{
		//Ĭ�ϴӵ�0�ų�����ʼ
		activeSceneNum = 0;
	}

	Ref<Scene> SceneManager::SetSceneActive(int num)
	{
		activeSceneNum = num;
		return scenes[num];
	}

	bool SceneManager::AddScene(Ref<Scene> scene)
	{
		scenes.push_back(scene);
		return true;
	}

	Ref<entt::registry> SceneManager::GetRegistry()
	{
		return scenes[activeSceneNum]->GetRegistry();
	}

	Ref<Scene> SceneManager::GetSceneActive()
	{
		return scenes[activeSceneNum];
	}
	
}
