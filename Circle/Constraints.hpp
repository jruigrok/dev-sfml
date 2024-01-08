#pragma once
#include <iostream>
#include <algorithm>
#include <Circle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

class Link
{
public:
	
	Link(Circle* ob1_, Circle* ob2_, float dist_, float rigigity_) :
		ob1(ob1_), ob2(ob2_), dist(dist_), rigigity(rigigity_)
	{};

	Link(Circle* ob1_, Circle* ob2_, sf::Vector2f pos1, sf::Vector2f pos2, float rigigity_) :
		ob1(ob1_), ob2(ob2_), rigigity(rigigity_)
	{
		const sf::Vector2f axis = pos1 - pos2;
		dist = sqrt(axis.x * axis.x + axis.y * axis.y);
	};

	~Link() = default;

	void update() {
		const sf::Vector2f axis = ob1->pos - ob2->pos;
		const float d = sqrt(axis.x * axis.x + axis.y * axis.y);
		const sf::Vector2f dir = axis / dist;
		const float delta = dist - d > 0 ? std::min(std::abs(dist - d), MAXMOVMENT) : -std::min(std::abs(dist - d), MAXMOVMENT);
		ob1->pos += 0.5f * delta * (1 + ob2->holdPos) * !ob1->holdPos * dir * rigigity;
		ob2->pos -= 0.5f * delta * (1 + ob1->holdPos) * !ob2->holdPos * dir * rigigity;
	}
private:
	float dist;
	const float rigigity;
	Circle* ob1;
	Circle* ob2;
	const float MAXMOVMENT = 3.0f;
};