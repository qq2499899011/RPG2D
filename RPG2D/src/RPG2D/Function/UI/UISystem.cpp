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
		//创建上下文
		ImGui::CreateContext();
		//基本设置
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		//设置字体
		io.FontDefault = io.Fonts->AddFontFromFileTTF("C:/Work/GameEngine/RPG2D/Sandbox/asset/font/simhei.ttf", m_FontSize);
		// imgui颜色
		ImGui::StyleColorsDark();
		//初始化glfw和opengl
		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	//新帧
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
		//设置清空颜色
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		//获取窗口信息
		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		int window_x, window_y;
		//获取window的位置
		glfwGetWindowPos(window,&window_x,&window_y);
		//获取window大小
		ImVec2 glfwSize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
		//相对GLFW窗口的位置和大小
		// 设置窗口的位置和大小
		ImGui::SetNextWindowPos(ImVec2(window_x,window_y), ImGuiCond_Always);  // 绝对屏幕位置 (100, 100)
		ImGui::SetNextWindowSize(glfwSize, ImGuiCond_Always); // 窗口大小为 (300, 200)
		//窗口透明
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0)); // 将背景颜色设为完全透明
		//开始UI绘制
		ImGui::Begin("Main UI Window", nullptr, ImGuiWindowFlags_NoTitleBar  | ImGuiWindowFlags_NoScrollbar);
		entt::registry* m_Registry = GlobalContext::GetInstance()->m_SceneManager->GetRegistry();
		//遍历所有带有Button组件的实体
		auto group1 = m_Registry->view<TransformComponent,ButtonComponent>();
		for (auto entity : group1)
		{
			auto [trans, button] = group1.get<TransformComponent, ButtonComponent>(entity);
			//根据当前位置绘制按钮
			ImGui::SetCursorPos(ImVec2(trans.Translation.x,trans.Translation.y));
			//设置颜色
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(button.color.r, button.color.g, button.color.b, button.color.a));
			//获取字体缩放比例
			ImGui::SetWindowFontScale(button.fontsize/m_FontSize);
			if (ImGui::Button(button.context.c_str(), ImVec2(trans.Scale.x, trans.Scale.y))) {
				//按下后调用函数
				button.buttonClick();
			}
			ImGui::PopStyleColor();
		}
		//遍历所有带有Text组件的实体
		auto group2 = m_Registry->view<TransformComponent,TextComponent>();
		if (!group2.empty()) {
			for (auto entity : group2)
			{
				auto [trans, text] = group2.get<TransformComponent, TextComponent>(entity);
				//根据当前位置绘制按钮
				ImGui::SetCursorPos(ImVec2(trans.Translation.x,trans.Translation.y));
				ImGui::SetWindowFontScale(text.fontsize/m_FontSize);
				ImGui::Text(text.context.c_str());
			}
		}
		//遍历带有进度条组件的实体
		auto group3 = m_Registry->view<TransformComponent, ProgressBarComponent>();
		if (!group3.empty()) {
			for (auto entity : group3)
			{
				auto [trans, progress] = group3.get<TransformComponent, ProgressBarComponent>(entity);
				//根据当前位置绘制按钮
				ImGui::SetCursorPos(ImVec2(trans.Translation.x, trans.Translation.y));
				ImGui::SetWindowFontScale(progress.fontsize/m_FontSize);
				ImGui::ProgressBar(progress.percent,ImVec2(trans.Scale.x,trans.Scale.y),progress.context.c_str());
			}
		}
		//结束绘制
		ImGui::End();
		ImGui::PopStyleColor(); // 恢复原始背景颜色
		End();
	}
}
