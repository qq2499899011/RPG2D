#include "RPG2Dpch.h"
#include "SceneManager.h"
namespace RPG2D {
	SceneManager::SceneManager()
	{
		//Ĭ�ϴӵ�0�ų�����ʼ
		activeSceneNum = 0;
	}

	//���û�Ծ����
	Ref<Scene> SceneManager::SetSceneActive(int num)
	{
		activeSceneNum = num;
		//���ú��ʼ��������
		GlobalContext::GetInstance()->m_PhysicsSystem->Init();
		return scenes[num];
	}
	//ͨ�������������û�Ծ����
	Ref<Scene> SceneManager::SetSceneActive(const std::string& name) {
		for (size_t i = 0; i < scenes.size(); i++) {
			if ((scenes[i]->GetName()).compare(name) == 0) {
				//�Ƚ������Ƿ�һ��
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
				//�Ƚ������Ƿ�һ��
				return SetNextScene(i);
			}
		}
	}

	void SceneManager::SwitchScene()
	{
		//�鿴��ǰ�Ƿ���Ҫ�л�����
		if (nextSceneNum != -1) {
			SetSceneActive(nextSceneNum);
			nextSceneNum = -1;
		}
	}
	
}
