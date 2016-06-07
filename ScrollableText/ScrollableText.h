#pragma once
#include "IWidget.h"
#include <string>
#include <SDL.h>
#include <unordered_map>

class ScrollableText : public IWidget
{
public:
	ScrollableText(SDL_Renderer *renderer, const std::string font, int width, int height);
	virtual ~ScrollableText();
	virtual void update(const Uint8 *keystate, int mousewheel);
	virtual void render(int x_pos, int y_pos);
	virtual void set_text(std::string text);
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
	std::unordered_map<char, SDL_Rect> _font_coords;
};