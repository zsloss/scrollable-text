#pragma once
#include "Widget.h"
#include <string>
#include <SDL.h>
#include <unordered_map>
#include "Utils.h"
#include <vector>
#include <memory>
#include "Font.h"

class ScrollableText : public Widget
{
public:
	ScrollableText(SDL_Renderer *renderer, const std::string font, int x_pos, int y_pos, int width, int height);
	virtual ~ScrollableText();
	virtual void render();
	virtual void set_text(std::string text);
	virtual void scroll_up();
	virtual void scroll_down();
	virtual void click(int x, int y);
private:
	bool get_texture(std::string text);
	void move_text_y(double y);
	bool _scrollable;
	std::string _text;
	SDL_Renderer *_renderer;
	int _text_x, _text_y, _text_width, _text_height;
	SDL_Texture *_texture;
	std::vector<SDL_Rect> _links;
	std::shared_ptr<Font> _font;
};