#include "RPG2Dpch.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace RPG2D {
	/// <summary>
	/// 通过顶点着色器和片段着色器生成shader
	/// </summary>
	/// <param name="name"></param>
	/// <param name="vertexSrc"></param>
	/// <param name="fragmentSrc"></param>
	OpenGLShader::OpenGLShader(const char* vertexSrc, const char* fragmentSrc,const char* gSrc )
	{
		Compile(vertexSrc,fragmentSrc,gSrc);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(ID);
	}

	/// <summary>
	/// 根据类型-源码组成的键值对，编译生成program
	/// </summary>
	/// <param name="shaderSources"></param>
	void OpenGLShader::Compile(const char* vertexSrc, const char* fragmentSrc,const char* geometrySrc )
	{
		unsigned int sVertex, sFragment, gShader;
		// vertex Shader
		sVertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(sVertex, 1, &vertexSrc, NULL);
		glCompileShader(sVertex);
		checkCompileErrors(sVertex, "VERTEX");
		// fragment Shader
		sFragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(sFragment, 1, &fragmentSrc, NULL);
		glCompileShader(sFragment);
		checkCompileErrors(sFragment, "FRAGMENT");
		// if geometry shader source code is given, also compile geometry shader
		if (geometrySrc != nullptr)
		{
			gShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(gShader, 1, &geometrySrc, NULL);
			glCompileShader(gShader);
			checkCompileErrors(gShader, "GEOMETRY");
		}
		// shader program
		this->ID = glCreateProgram();
		glAttachShader(this->ID, sVertex);
		glAttachShader(this->ID, sFragment);
		if (geometrySrc != nullptr)
			glAttachShader(this->ID, gShader);
		glLinkProgram(this->ID);
		checkCompileErrors(this->ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(sVertex);
		glDeleteShader(sFragment);
		if (geometrySrc != nullptr)
			glDeleteShader(gShader);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(ID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		Bind();
		GLint location = glGetUniformLocation(ID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		Bind();
		GLint location = glGetUniformLocation(ID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		Bind();
		GLint location = glGetUniformLocation(ID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		Bind();

		GLint location = glGetUniformLocation(ID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		Bind();

		GLint location = glGetUniformLocation(ID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		Bind();

		GLint location = glGetUniformLocation(ID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& value)
	{
		GLint location = glGetUniformLocation(ID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		Bind();

		GLint location = glGetUniformLocation(ID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::checkCompileErrors(unsigned int object, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(object, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(object, 1024, NULL, infoLog);
				RPG2D_CORE_ERROR( "SHADER: Compile-time error: Type:'{0}','{1}'",type,infoLog);
			}
		}
		else
		{
			glGetProgramiv(object, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(object, 1024, NULL, infoLog);
				RPG2D_CORE_ERROR("Shader: Link - time error : Type:'{0}','{1}'",type,infoLog);
			}
		}
	}
	uint32_t OpenGLShader::GetID() const
	{
		return ID;
	}
}
