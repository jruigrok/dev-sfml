#pragma once
#include <iostream>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

struct Circle
{
public:
	sf::Vector2f pos;
	bool holdPos = false;

	Circle(sf::Vector2f Pos, sf::Vector2f v)
		:
		pos(Pos),
		oldPos(Pos - v),
		color(sf::Color::White)
	{};

	Circle(sf::Vector2f Pos, sf::Vector2f v, sf::Color Color)
		:
		pos(Pos),
		oldPos(Pos - v),
		color(Color)
	{};

	void updatePos(float dt) {
		if (!holdPos) {
			sf::Vector2f v = pos - oldPos;
			oldPos = pos;
			pos += v + a * dt * dt;
		}
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

	sf::Color getColor() const {
		return color;
	}

private:
	sf::Vector2f oldPos;
	sf::Vector2f a = { 0,0 };
	sf::Color color;
};