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
		//����������������
		GlobalContext::GetInstance()->m_PhysicsSystem->WhenActiveScene();
		return scenes[num];
	}
	Ref<Scene> SceneManager::CreateScene(const std::string& name)
	{
		Ref<Scene> scene = CreateRef<Scene>(name);
		//����������
		//���ú��ʼ��������
		scene->SetPhysicsWorld(GlobalContext::GetInstance()->m_PhysicsSystem->Create());
		//���볡��
		AddScene(scene);
		return scene;
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

	void SceneManager::DeInit()
	{
		//�ͷ����г���
		for (Ref<Scene> scene : scenes) {
			//�����ͷ���
			scene->DeInit();
		}
		scenes.clear();
	}
	
}
