#include "InputManager.h"

void InputManager::process_event(SDL_Event * e)
{
	if (e->type == SDL_MOUSEMOTION) {
		int mx, my;
		SDL_GetMouseState(&mx, &my);
		focused_widget = nullptr;
		for (auto widget : widgets) {
			if (mx >= widget->get_x() && mx <= widget->get_x() + widget->get_width() &&
				my >= widget->get_y() && my <= widget->get_y() + widget->get_height()) {
				focused_widget = widget;
				break;
			}
		}
	}
	else if (focused_widget != nullptr) {
		if (e->type == SDL_KEYDOWN) {
			switch (e->key.keysym.sym) {
			case SDLK_UP:
				focused_widget->scroll_up(); break;
			case SDLK_DOWN:
				focused_widget->scroll_down(); break;
			}
		}
		else if (e->type == SDL_MOUSEBUTTONDOWN) {
			int mx, my;
			SDL_GetMouseState(&mx, &my);
			focused_widget->click(mx, my);
		}
		else if (e->type == SDL_MOUSEWHEEL) {
			auto mousewheel = e->wheel.y;
			if (mousewheel > 0) {
				focused_widget->scroll_up();
			}
			else if (mousewheel < 0) {
				focused_widget->scroll_down();
			}
		}
	}
}

void InputManager::add_widget(std::shared_ptr<Widget> widget)
{
	widgets.push_back(widget);
}
