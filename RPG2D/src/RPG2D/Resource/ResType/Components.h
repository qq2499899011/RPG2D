#pragma once

#include "RPG2D/Core/UID.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <string>
#include "RPG2D/Function/Renderer/Texture2D.h"
#include "RPG2D/Function/Animation/AnimatiorController.h"
namespace RPG2D {

	struct IDComponent
	{
		UID ID;
		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;
		bool mirror = false;
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	//动画组件
	struct AnimatiorControllerComponent
	{
		//指向AnimatorController
		Ref<AnimatiorController> animatiorController;
		AnimatiorControllerComponent() = default;
		AnimatiorControllerComponent(const AnimatiorControllerComponent&) = default;
		AnimatiorControllerComponent(Ref<AnimatiorController> animatior): animatiorController(animatior){}
	};
	// Forward declaration
	class ScriptableEntity;

	//脚本组件,使用这个组件都要执行一次Bind函数。
	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;
		//实例化脚本
		ScriptableEntity* (*InstantiateScript)();
		//摧毁脚本
		void (*DestroyScript)(NativeScriptComponent*);
		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	struct CircleRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float Thickness = 1.0f;
		float Fade = 0.005f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

	/*
	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};
	*/
	// Physics
	struct Rigidbody2DComponent
	{
		//刚体类型
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType Type = BodyType::Static;
		//固定旋转
		bool FixedRotation = true;

		// Storage for runtime
		void* RuntimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

	//碰撞体的size可以和物体的长宽一致
	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		// TODO(Yan): move into physics material in the future maybe
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;//默认完全不弹性
		float RestitutionThreshold = 0.5f;//只有速度超过这个值的物体相撞时，才会发生碰撞
		bool isSensor = false;
		// Storage for runtime
		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	struct CircleCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;

		// TODO(Yan): move into physics material in the future maybe
		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		// Storage for runtime
		void* RuntimeFixture = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
	};
	using ButtonCallBack = std::function<void()>;
	//按钮组件
	struct ButtonComponent
	{
		//按钮名称
		std::string context;
		//按钮函数
		ButtonCallBack buttonClick;
		//颜色
		glm::vec4 color = glm::vec4(1.0f,0.0f,0.0f,1.0f);
		ButtonComponent() = default;
		ButtonComponent(const ButtonComponent&) = default;
		ButtonComponent(const std::string& tag)
			: context(tag) {}
		void Bind(ButtonCallBack callback) {
			//直接赋值即可
			buttonClick = callback;
		}
		//直接调用即可
		void OnClick() {
			buttonClick();
		}
	};
	//文本组件
	struct TextComponent
	{
		//文本内容
		std::string context;
		glm::vec4 color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		TextComponent() = default;
		TextComponent(const TextComponent&) = default;
	};

	//进度条组件，可以用来显示血条等信息
	struct ProgressBarComponent {
		//名称
		std::string context;
		//百分比
		float percent;
		//颜色
		glm::vec4 color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		ProgressBarComponent() = default;
		ProgressBarComponent(const ProgressBarComponent&) = default;
	};
	/*
	struct TextComponent
	{
		std::string TextString;
		Ref<Font> FontAsset = Font::GetDefault();
		glm::vec4 Color{ 1.0f };
		float Kerning = 0.0f;
		float LineSpacing = 0.0f;
	};
	*/

	template<typename... Component>
	struct ComponentGroup
	{
	};

	/// <summary>
	/// 不包括IDComponent 和 tagComponent
	/// </summary>
	using AllComponents =
		ComponentGroup<TransformComponent, SpriteRendererComponent,
		CircleRendererComponent,SpriteRendererComponent,NativeScriptComponent,
	    Rigidbody2DComponent, BoxCollider2DComponent,ButtonComponent,TextComponent,ProgressBarComponent,
		CircleCollider2DComponent>;

}
