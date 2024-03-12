#pragma once

#include "RPG2D/Function/Renderer/Shader.h"
#include <glm/glm.hpp>

// TODO: REMOVE!
typedef unsigned int GLenum;

namespace RPG2D {

	class OpenGLShader : public Shader
	{
	public:
		//ͨ���ļ�ֱ�Ӽ��ػ���ͨ��Դ��
		OpenGLShader(const char* vertexSrc, const char* fragmentSrc,const char* gSrc );
		virtual ~OpenGLShader();

		//�󶨺ͽ��
		virtual void Bind() const override;
		virtual void Unbind() const override;

		//����uniform��ֵ
		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;
		virtual uint32_t GetID() const override;

	private:
		void Compile(const char* vertexSrc, const char* fragmentSrc,const char* geometrySrc );
		void checkCompileErrors(unsigned int object, std::string type);
	private:
		//sahder��id������
		uint32_t ID;
		// ͨ�� Shader �̳�
	};

}
