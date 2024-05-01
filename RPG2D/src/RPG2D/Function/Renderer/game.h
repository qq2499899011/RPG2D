/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H
#include <vector>
#include <tuple>
#include <glm/glm.hpp>
#include "RPG2D/Resource/AssetManager/AssetManager.h"

namespace RPG2D
{
    // Initial size of the player paddle
	const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
	// Initial velocity of the player paddle
	const float PLAYER_VELOCITY(500.0f);
	// Initial velocity of the Ball
	const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
	// Radius of the ball object
	const float BALL_RADIUS = 12.5f;

	// Game holds all game-related state and functionality.
	// Combines all game-related data into a single class for
	// easy access to each of the components and manageability.
	class Game
	{
	public:
		unsigned int            Width, Height;
		unsigned int            Lives;
		AssetManager* assetManager;
		// constructor/destructor
		Game(unsigned int width, unsigned int height);
		~Game();
		// initialize game state (load all shaders/textures/levels)
		void Init();
		void Render();
};



}
#endif