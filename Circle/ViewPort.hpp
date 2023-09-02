#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

class ViewPort {
public:
	ViewPort(sf::RenderStates& states_, sf::RenderStates& objectStates_, sf::Vector2f pos_, float zoom_) : 
		states(states_), objectStates(objectStates_), pos(pos_), scale(zoom_) {
		transform.translate(pos_);
		transform.scale(zoom_, zoom_);
	};

	void updateStates(sf::Transform& transform) {
		states.transform = transform;
		objectStates.transform = transform;
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

	float getScale() {
		return scale;
	}

	sf::RenderStates& states;
	sf::RenderStates& objectStates;
	
private:
	
	sf::Transform transform;
	sf::Vector2f pos;
	float scale;
};
