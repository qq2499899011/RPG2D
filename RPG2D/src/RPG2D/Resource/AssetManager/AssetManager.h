/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>


#include "RPG2D/Function/Renderer/Shader.h"
#include "RPG2D/Function/Renderer/Texture2D.h"

/// <summary>
/// 从根据传入的着色器文件路径生成shdaer
/// 根据传入图片路径生成texture
/// 根据名称获取shader和texture
/// </summary>
namespace RPG2D {
class AssetManager
{
	public:
		// resource storage 
		std::map<std::string, Ref<Shader>> Shaders;
		std::map<std::string, Ref<Texture2D>> Textures;
		// loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
		Ref<Shader>    LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, std::string name);
		// retrieves a stored sader
		Ref<Shader>    GetShader(std::string name);
		// loads (and generates) a texture from file
		Ref<Texture2D> LoadTexture(const char* file, bool alpha,std::string name);
		// retrieves a stored texture
		Ref<Texture2D> GetTexture(std::string name);
		// properly de-allocates all loaded resources
		void      Clear();
		AssetManager();
	private:
		// private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
		// loads and generates a shader from file
		Ref<Shader>    loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
		// loads a single texture from file
		Ref<Texture2D> loadTextureFromFile(const char *file, bool alpha);
	};
}

#endif