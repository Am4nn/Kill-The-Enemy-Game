#pragma once

#include "Common.h"

class CTransform
{
public:
	Vec2 pos = { 0.0f, 0.0f };
	Vec2 velocity = { 0.0f, 0.0f };
	float angle = 0.0f;

	CTransform(const Vec2& pos, const Vec2& velocity, float angle)
		: pos(pos), velocity(velocity), angle(angle) {}
};

class CShape
{
public:
	sf::CircleShape circle;

	CShape(float radius, int points, const sf::Color& fillColor, const sf::Color& outlineColor, float thickness)
		:circle(radius, points)
	{
		circle.setFillColor(fillColor);
		circle.setOutlineColor(outlineColor);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	}
};

class CCollision
{
public:
	float radius = 0.0f;
	CCollision() {}
	CCollision(float radius)
		: radius(radius) {}
};

class CScore
{
public:
	int score = 0;
	CScore(int score)
		:score(score){}
};

class CLifespane
{
public:
	int remainingLife = 0;
	int totalLife = 0;
	CLifespane() {}
	CLifespane(int totalLife)
		:totalLife(totalLife), remainingLife(totalLife) {}
};

class CInput
{
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool shoot = false;

	CInput() {}
};