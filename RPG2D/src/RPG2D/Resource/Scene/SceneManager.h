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
		bool AddScene(Ref<Scene> scene);
		Ref<entt::registry> GetRegistry();
		Ref<Scene> GetSceneActive();
	private:
		int activeSceneNum;
		std::vector<Ref<Scene>> scenes;
	};
}

