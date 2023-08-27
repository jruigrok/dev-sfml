#pragma once
#include <iostream>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

class Circle
{
public:
	sf::Vector2f pos;

	Circle(sf::Vector2f Pos, sf::Vector2f v)
		:
		pos(Pos),
		oldPos(Pos - v) {};

	void updatePos(float dt) {
		sf::Vector2f v = pos - oldPos;
		oldPos = pos;
		pos += v + a * dt * dt;
		a = { 0,0 };
	}

	void setVelocity(sf::Vector2f v) {
		oldPos = pos - v;
	}

	void set_a(sf::Vector2f a_) {
		a = a_;
	}

	void setPos(sf::Vector2f pos_) {
		oldPos += pos_ - pos;
		pos = pos_;
	}

	void movePos(sf::Vector2f pos_) {
		pos += pos_;
		oldPos += pos_;
	}

private:
	sf::Vector2f oldPos;
	sf::Vector2f a = { 0,0 };
};