#include "RPG2Dpch.h"
#include "UISystem.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "RPG2D/Core/Application.h"
#include "RPG2D/Resource/ResType/Entity.h"

// TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>
namespace RPG2D {
	void UISystem::Init()
	{
		IMGUI_CHECKVERSION();
		//����������
		ImGui::CreateContext();
		//��������
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		//��������
		io.FontDefault = io.Fonts->AddFontFromFileTTF("C:/Work/GameEngine/RPG2D/Sandbox/asset/font/simhei.ttf", m_FontSize);
		// imgui��ɫ
		ImGui::StyleColorsDark();
		//��ʼ��glfw��opengl
		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	//��֡
	void UISystem::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void UISystem::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void UISystem::Update(Timestep)
	{
		Begin();
		//���������ɫ
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		//��ȡ������Ϣ
		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		int window_x, window_y;
		//��ȡwindow��λ��
		glfwGetWindowPos(window,&window_x,&window_y);
		//��ȡwindow��С
		ImVec2 glfwSize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
		//���GLFW���ڵ�λ�úʹ�С
		// ���ô��ڵ�λ�úʹ�С
		ImGui::SetNextWindowPos(ImVec2(window_x,window_y), ImGuiCond_Always);  // ������Ļλ�� (100, 100)
		ImGui::SetNextWindowSize(glfwSize, ImGuiCond_Always); // ���ڴ�СΪ (300, 200)
		//����͸��
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0)); // ��������ɫ��Ϊ��ȫ͸��
		//��ʼUI����
		ImGui::Begin("Main UI Window", nullptr, ImGuiWindowFlags_NoTitleBar  | ImGuiWindowFlags_NoScrollbar);
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		//�������д���Button�����ʵ��
		auto group1 = m_Registry->view<TransformComponent,ButtonComponent>();
		for (auto entity : group1)
		{
			auto [trans, button] = group1.get<TransformComponent, ButtonComponent>(entity);
			//���ݵ�ǰλ�û��ư�ť
			ImGui::SetCursorPos(ImVec2(trans.Translation.x,trans.Translation.y));
			//������ɫ
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(button.color.r, button.color.g, button.color.b, button.color.a));
			//��ȡ�������ű���
			ImGui::SetWindowFontScale(button.fontsize/m_FontSize);
			if (ImGui::Button(button.context.c_str(), ImVec2(trans.Scale.x, trans.Scale.y))) {
				//���º���ú���
				button.buttonClick();
			}
			ImGui::PopStyleColor();
		}
		//�������д���Text�����ʵ��
		auto group2 = m_Registry->view<TransformComponent,TextComponent>();
		if (!group2.empty()) {
			for (auto entity : group2)
			{
				auto [trans, text] = group2.get<TransformComponent, TextComponent>(entity);
				//���ݵ�ǰλ�û��ư�ť
				ImGui::SetCursorPos(ImVec2(trans.Translation.x,trans.Translation.y));
				ImGui::SetWindowFontScale(text.fontsize/m_FontSize);
				ImGui::Text(text.context.c_str());
			}
		}
		//�������н����������ʵ��
		auto group3 = m_Registry->view<TransformComponent, ProgressBarComponent>();
		if (!group3.empty()) {
			for (auto entity : group3)
			{
				auto [trans, progress] = group3.get<TransformComponent, ProgressBarComponent>(entity);
				//���ݵ�ǰλ�û��ư�ť
				ImGui::SetCursorPos(ImVec2(trans.Translation.x, trans.Translation.y));
				ImGui::SetWindowFontScale(progress.fontsize/m_FontSize);
				ImGui::ProgressBar(progress.percent,ImVec2(trans.Scale.x,trans.Scale.y),progress.context.c_str());
			}
		}
		//��������
		ImGui::End();
		ImGui::PopStyleColor(); // �ָ�ԭʼ������ɫ
		End();
	}
}
