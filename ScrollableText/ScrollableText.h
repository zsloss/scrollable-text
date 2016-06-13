#pragma once
#include "IWidget.h"
#include <string>
#include <SDL.h>
#include <unordered_map>
#include "Utils.h"
#include <vector>

class ScrollableText : public IWidget
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
	std::string _text;
	SDL_Renderer *_renderer;
	double _text_x, _text_y;
	int _text_width, _text_height;
	SDL_Texture *_texture;
	SDL_Texture *_font;
	bool load_font_texture(const std::string filename);
	utils::Font_info _font_info;
	std::vector<SDL_Rect> _links;
};