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
		//更新物理引擎内容
		GlobalContext::GetInstance()->m_PhysicsSystem->WhenActiveScene();
		return scenes[num];
	}
	Ref<Scene> SceneManager::CreateScene(const std::string& name)
	{
		Ref<Scene> scene = CreateRef<Scene>(name);
		//设置物理场景
		//设置后初始化物理场景
		scene->SetPhysicsWorld(GlobalContext::GetInstance()->m_PhysicsSystem->Create());
		//加入场景
		AddScene(scene);
		return scene;
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

	void SceneManager::SetNextScene(int next)
	{
		nextSceneNum = next;
	}

	void SceneManager::SetNextScene(const std::string& name)
	{
		for (size_t i = 0; i < scenes.size(); i++) {
			if ((scenes[i]->GetName()).compare(name) == 0) {
				//比较名称是否一致
				return SetNextScene(i);
			}
		}
	}

	void SceneManager::SwitchScene()
	{
		//查看当前是否需要切换场景
		if (nextSceneNum != -1) {
			SetSceneActive(nextSceneNum);
			nextSceneNum = -1;
		}
	}

	void SceneManager::DeInit()
	{
		//释放所有场景
		for (Ref<Scene> scene : scenes) {
			//调用释放我
			scene->DeInit();
		}
		scenes.clear();
	}
	
}
