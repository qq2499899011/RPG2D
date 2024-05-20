#pragma once
#include <RPG2D.h>
using namespace RPG2D;
class UI
{
public:
	//��װ�½�һЩ��ť
	static void Assemble() {
		Ref<Scene> scene = GlobalContext::GetInstance()->m_SceneManager->GetSceneActive();
		Entity button = scene->CreateEntity("Button");
		//����transform���
		TransformComponent buttonTrans;
		buttonTrans.Translation = glm::vec3{ 0,0,0.3f };
		buttonTrans.Scale = glm::vec3{ 50,50,1 };
		scene->GetComponentWithName<TransformComponent>("Button") = buttonTrans;
		//�󶨰�ť���
		ButtonComponent buttonComponent;
		buttonComponent.context = "Helloworld";
		buttonComponent.Bind(&UI::OnClick);
		button.AddComponent<ButtonComponent>(buttonComponent);

		//��һ��Ѫ��
		Entity hpbar = scene->CreateEntity("HP");
		//����transform���
		TransformComponent hpbarTrans;
		hpbarTrans.Translation = glm::vec3{ 200,200,0.3f };
		hpbarTrans.Scale = glm::vec3{ 100,20,1 };
		scene->GetComponentWithName<TransformComponent>("HP") = hpbarTrans;
		//�󶨰�ť���
		ProgressBarComponent hpbarComponent;
		hpbarComponent.context = "HP";
		hpbarComponent.percent = 1.0f;
		hpbar.AddComponent<ProgressBarComponent>(hpbarComponent);
	}
	static void OnClick() {
		RPG2D_INFO("button click");
	}
};

