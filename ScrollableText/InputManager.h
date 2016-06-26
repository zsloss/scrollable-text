#pragma once
#include <SDL.h>
#include <vector>
#include "Widget.h"
#include <memory>

class InputManager
{
public:
	InputManager() : focused_widget(nullptr) {}
	virtual ~InputManager() {}
	void process_event(SDL_Event *e);
	void add_widget(std::shared_ptr<Widget> widget);
private:
	std::vector<std::shared_ptr<Widget>> widgets;
	std::shared_ptr<Widget> focused_widget;
};

