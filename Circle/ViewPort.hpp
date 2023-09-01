#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>

class ViewPort {
public:
	ViewPort(sf::RenderStates& states_, sf::Vector2f pos_, float zoom_) : states(states_), pos(pos_), scale(zoom_) {
		transform.translate(pos_);
		transform.scale(zoom_, zoom_);
		states.transform = transform;
	};

	void zoom(float factor) {
		transform.scale(factor, factor);
		scale *= factor;
		states.transform = transform;
	}

	void move(sf::Vector2f pos_) {
		transform.translate(pos_ / scale);
		pos += pos_;
		states.transform = transform;
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

	void handleEvents(sf::Event& event) {
		if (event.type == sf::Event::MouseWheelMoved) {
			if (event.mouseWheel.delta < 0) {
				zoomOnPoint(scrollZoomMag, { (float)event.mouseWheel.x, (float)event.mouseWheel.y });
			}
			else {
				zoomOnPoint(1.0f / scrollZoomMag, { (float)event.mouseWheel.x, (float)event.mouseWheel.y });
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed) {
			if (!mouseDown) {
				anchorPos = getTruePos({ (float)event.mouseButton.x, (float)event.mouseButton.y });
				
			}
			mouseDown = true;
		}
		else if (event.type == sf::Event::MouseButtonReleased) {
			mouseDown = false;
		}
		if (event.type == sf::Event::MouseMoved && mouseDown) {
			mousePos = { (float)event.mouseMove.x, (float)event.mouseMove.y };
			move((getTruePos(mousePos) - anchorPos) * scale);
		}

	}
	
private:
	sf::RenderStates& states;
	sf::Transform transform;
	sf::Vector2f pos;
	sf::Vector2f anchorPos = { 0,0 };
	sf::Vector2f mousePos = { 0,0 };
	float scale;
	float scrollZoomMag = 0.8f;
	bool mouseDown = false;
	bool mouseOnScreen = true;
};
