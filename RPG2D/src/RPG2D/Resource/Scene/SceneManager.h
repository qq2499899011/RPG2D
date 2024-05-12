#pragma once
#include "Scene.h"
/// <summary>
/// ≥°æ∞π‹¿Ì
/// </summary>
namespace RPG2D {
	class SceneManager
	{
	public:
		SceneManager();
		Ref<Scene> SetSceneActive(int num);
		Ref<Scene> SetSceneActive(const std::string& name);
		bool AddScene(Ref<Scene> scene);
		entt::registry* GetRegistry();
		Ref<Scene> GetSceneActive();
		void SetNextScene(int next);
		void SetNextScene(const std::string& name);
		void SwitchScene();
	private:
		int activeSceneNum;
		int nextSceneNum = -1;
		std::vector<Ref<Scene>> scenes;
	};
}

