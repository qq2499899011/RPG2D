#pragma once
#include <RPG2D.h>
using namespace RPG2D;
class UI
{
public:
	//组装新建一些按钮
	static void Assemble() {
		Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
		Entity button = scene->CreateEntity("Button");
		//设置transform组件
		TransformComponent buttonTrans;
		buttonTrans.Translation = glm::vec3{ 0,0,0.3f };
		buttonTrans.Scale = glm::vec3{ 50,50,1 };
		scene->GetComponentWithName<TransformComponent>("Button") = buttonTrans;
		//绑定按钮组件
		ButtonComponent buttonComponent;
		buttonComponent.context = "Helloworld";
		buttonComponent.Bind(&UI::OnClick);
		button.AddComponent<ButtonComponent>(buttonComponent);

		//加一个血条
		Entity hpbar = scene->CreateEntity("HP");
		//设置transform组件
		TransformComponent hpbarTrans;
		hpbarTrans.Translation = glm::vec3{ 200,200,0.3f };
		hpbarTrans.Scale = glm::vec3{ 100,20,1 };
		scene->GetComponentWithName<TransformComponent>("HP") = hpbarTrans;
		//绑定按钮组件
		ProgressBarComponent hpbarComponent;
		hpbarComponent.context = "HP";
		hpbarComponent.percent = 1.0f;
		hpbar.AddComponent<ProgressBarComponent>(hpbarComponent);
	}
	static void OnClick() {
		RPG2D_INFO("button click");
	}
};

