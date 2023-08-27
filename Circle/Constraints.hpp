#pragma once
#include <iostream>
#include <Circle.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

class Link
{
public:
	Link(uint64_t idx1_, uint64_t idx2_, float dist_, float rigigity_) :
		idx1(idx1_), idx2(idx2_), dist(dist_), rigigity(rigigity_)
	{};

	Link(uint64_t idx1_, uint64_t idx2_, sf::Vector2f pos1, sf::Vector2f pos2, float rigigity_) :
		idx1(idx1_), idx2(idx2_), rigigity(rigigity_)
	{
		const sf::Vector2f axis = pos1 - pos2;
		dist = sqrt(axis.x * axis.x + axis.y * axis.y);
	};

	~Link() = default;

	void update(std::vector<Circle>& circles) {
		Circle& ob1 = circles[idx1];
		Circle& ob2 = circles[idx2];
		const sf::Vector2f axis = ob1.pos - ob2.pos;
		const float d = sqrt(axis.x * axis.x + axis.y * axis.y);
		const sf::Vector2f dir = axis / dist;
		const float delta = dist - d;
		ob1.pos += 0.5f * delta * dir * rigigity;
		ob2.pos -= 0.5f * delta * dir * rigigity;
	}
private:
	float dist;
	const float rigigity;
	const uint64_t idx1;
	const uint64_t idx2;
};

class Pin
{
public:

	Pin(uint64_t idx_, sf::Vector2f pos_) :
		idx(idx_), pos(pos_)
	{};

	void update(std::vector<Circle>& circles) {
		Circle& ob = circles[idx];
		ob.pos = pos;
	}

private:
	sf::Vector2f pos;
	uint64_t idx;
};
