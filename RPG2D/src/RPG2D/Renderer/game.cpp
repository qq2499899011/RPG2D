/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "RPG2Dpch.h"
#include <algorithm>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "game_object.h"


// Game-related State data
SpriteRenderer    *Renderer;
GameObject        *Player;


float ShakeTime = 0.0f;


Game::Game(unsigned int width, unsigned int height) 
    : Width(width), Height(height), Lives(3)
{ 

}

Game::~Game()
{
    delete Renderer;
    delete Player;
}
/// <summary>
/// 初始化 着色器 材质 渲染器
/// 设置着色器uniform
/// 生成物体,其实就是transform
/// </summary>
void Game::Init()
{
    // load shaders , 例子和
    ResourceManager::LoadShader("C:/Work/GameEngine/SimpEngine/Sandbox/src/sprite.vs", "C:/Work/GameEngine/SimpEngine/Sandbox/src/sprite.fs", nullptr, "sprite");
    // configure shaders , 设置project 矩阵
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // load textures
    ResourceManager::LoadTexture("C:/Work/GameEngine/SimpEngine/Sandbox/src/awesomeface.png", true, "face");
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // configure game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("face"));
}
void Game::Render()
{
	// draw background
	Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
	// draw player
	Player->Draw(*Renderer);
	std::stringstream ss; ss << this->Lives;
}

bool ShouldSpawn(unsigned int chance)
{
    unsigned int random = rand() % chance;
    return random == 0;
}
