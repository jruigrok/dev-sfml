#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <vector>

class ViewPort {
public:
	ViewPort(std::vector<sf::RenderStates*> statesV_, sf::Vector2f pos_, float zoom_) :
		statesV(statesV_) {
		zoom(zoom_);
		move(pos_);
	};

	void updateStates(sf::Transform& transform) {
		for (uint32_t i = 0; i < statesV.size(); i++) {
			statesV[i]->transform = transform;
		}
	}

	void zoom(float factor) {
		transform.scale(factor, factor);
		scale *= factor;
		updateStates(transform);
	}

	void move(sf::Vector2f pos_) {
		transform.translate(pos_ / scale);
		pos += pos_;
		updateStates(transform);
	}

	void moveTo(sf::Vector2f pos_) {
		move(pos_ - pos);
	}

	sf::Vector2f getTruePos(sf::Vector2f pos_) {
		return (pos_ - pos) / scale;
	}

	void zoomOnPoint(float factor, sf::Vector2f pos_) {
		const float rec = 1.0f - factor;
		sf::Vector2f truePos = getTruePos(pos_);
		move((truePos * rec) * scale);
		zoom(factor);
		
	}

	float getScale() const {
		return scale;
	}

private:

	std::vector<sf::RenderStates*> statesV;
	sf::Transform transform;
	sf::Vector2f pos = { 0,0 };
	float scale = 1;
};
