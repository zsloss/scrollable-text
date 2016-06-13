#pragma once
#include <SDL.h>
#include <vector>
#include "IWidget.h"
#include <memory>

class InputManager
{
public:
	InputManager() : focused_widget(nullptr) {}
	virtual ~InputManager() {}
	void process_event(SDL_Event *e);
	void add_widget(std::shared_ptr<IWidget> widget);
private:
	std::vector<std::shared_ptr<IWidget>> widgets;
	std::shared_ptr<IWidget> focused_widget;
};

