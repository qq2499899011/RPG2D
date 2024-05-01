/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "RPG2Dpch.h"
#include "game_object.h"

namespace RPG2D {
    GameObject::GameObject()
        : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite() { }

    GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Ref<Texture2D> sprite, glm::vec3 color, glm::vec2 velocity)
        : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite) { }

    void GameObject::Draw(SpriteRenderer& renderer)
    {
        renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
    }
}