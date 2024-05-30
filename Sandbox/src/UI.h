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
		buttonTrans.Scale = glm::vec3{ 200,50,1 };
		scene->GetComponentWithName<TransformComponent>("Button") = buttonTrans;
		//�󶨰�ť���
		ButtonComponent buttonComponent;
		buttonComponent.context = "Helloworld";
		buttonComponent.fontsize = 50.0f;
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
	//��װ�˵�UI
	static void MenuUIAssemble() {
		Ref<SceneManager> sceneManager = GlobalContext::GetInstance()->m_SceneManager;
		Ref<Scene> scene = sceneManager->GetSceneActive();
		Entity button = scene->CreateEntity("Play");
		//����transform���
		TransformComponent buttonTrans;
		//407
		buttonTrans.Translation = glm::vec3{ 508,589,0.3f };
		buttonTrans.Scale = glm::vec3{ 250,50,1 };
		button.GetComponent<TransformComponent>() = buttonTrans;
		//�󶨰�ť���
		ButtonComponent buttonComponent;
		buttonComponent.color = glm::vec4{0.0f,0.0f,0.0f,1.0f};
		buttonComponent.hoverColor = glm::vec4{0.75,0.75,0.75,1.0f};
		buttonComponent.context = "Start";
		buttonComponent.fontsize = 30.0f;
		buttonComponent.Bind(
			[sceneManager]() {
				//�����
				sceneManager->SetSceneActive("Start");
			}
		);
		button.AddComponent<ButtonComponent>(buttonComponent);
	}
	static void OnClick() {
		RPG2D_INFO("button click");
	}
};

