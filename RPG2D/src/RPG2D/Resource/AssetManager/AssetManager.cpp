/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "RPG2Dpch.h"
#include "AssetManager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <glad/glad.h>
#include <stb_image.h>

namespace RPG2D {
	AssetManager::AssetManager() { }

	Ref<Shader> AssetManager::LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name)
	{
		Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
		return Shaders[name];
	}

	Ref<Shader> AssetManager::GetShader(std::string name)
	{
		return Shaders[name];
	}

	Ref<Texture2D> AssetManager::LoadTexture(const char *file, bool alpha, std::string name)
	{
		Textures[name] = loadTextureFromFile(file, alpha);
		return Textures[name];
	}

	Ref<Texture2D> AssetManager::GetTexture(std::string name)
	{
		return Textures[name];
	}

	/// <summary>
	/// �ͷ�����shader��texture
	/// </summary>
	void AssetManager::Clear()
	{
		// (properly) delete all shaders	
		for (auto iter : Shaders)
			glDeleteProgram(iter.second->GetID());
		// (properly) delete all textures
		for (auto iter : Textures) {
			GLuint textureID = iter.second->GetID();
			glDeleteTextures(1, &textureID);
		}
	}

	Ref<Shader> AssetManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		try
		{
			// open files
			std::ifstream vertexShaderFile(vShaderFile);
			std::ifstream fragmentShaderFile(fShaderFile);
			if (!vertexShaderFile.is_open()) {
				RPG2D_CORE_ERROR("can't find vShaderFile");
			}
			if (!fragmentShaderFile.is_open()) {
				RPG2D_CORE_ERROR("can't find fShaderFile");
			}
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vertexShaderFile.rdbuf();
			fShaderStream << fragmentShaderFile.rdbuf();
			// close file handlers
			vertexShaderFile.close();
			fragmentShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
			// if geometry shader path is present, also load a geometry shader
			if (gShaderFile != nullptr)
			{
				std::ifstream geometryShaderFile(gShaderFile);
				std::stringstream gShaderStream;
				gShaderStream << geometryShaderFile.rdbuf();
				geometryShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
		catch (std::exception e)
		{
			RPG2D_CORE_ERROR("ERROR::SHADER: Failed to read shader files");
		}
		const char *vShaderCode = vertexCode.c_str();
		const char *fShaderCode = fragmentCode.c_str();
		const char *gShaderCode = geometryCode.c_str();
		//�ж����
		// 2. now create shader object from source code
		return Shader::Create(vShaderCode,fShaderCode,gShaderFile != nullptr ? gShaderCode : nullptr);
	}

	/// <summary>
	/// ���ļ��м��ز���
	/// </summary>
	/// <param name="file"></param>
	/// <param name="alpha"></param>
	/// <returns></returns>
	Ref<Texture2D> AssetManager::loadTextureFromFile(const char *file, bool alpha)
	{
		//���ȼ�鵱ǰͼƬ�Ƿ��Ѿ������ؽ�����assetmanager������Ѿ��������ˣ���ôֱ�ӷ���texture���С�
		std::string name = file;
		auto& it = Textures.find(name);
		if (it != Textures.end()) {
			// ������,ֱ�ӷ���texture����
			RPG2D_CORE_TRACE("TextureFound'{0}'",name);
			return it->second;
		}
		// ��������
		// create texture object
		Ref<Texture2D> texture = Texture2D::Create(name);
		if (alpha)
		{
			texture->SetImageFormat(GL_RGBA);
			texture->SetImageFormat(GL_RGBA);
		}
		// load image
		int width, height, nrChannels;
		unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
		if (data == nullptr) {
			RPG2D_CORE_ERROR("load image failed!");
		}
		// now generate texture
		texture->Generate(width, height, data);
		// and finally free image data
		stbi_image_free(data);
		return texture;
	}
}
