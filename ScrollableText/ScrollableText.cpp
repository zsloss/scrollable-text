#include "IWidget.h"
#include "ScrollableText.h"
#include <SDL.h>
#include <SDL_ttf.h>

ScrollableText::ScrollableText(SDL_Renderer *renderer, TTF_Font *font, int width, int height) : IWidget(width, height)
{
	_renderer = renderer;
	_font = font;
	_texture = nullptr;
}

ScrollableText::~ScrollableText()
{
	SDL_DestroyTexture(_texture);
	_texture = nullptr;
}

void ScrollableText::update(const Uint8 *keystate, int mousewheel)
{
	if (keystate[SDL_SCANCODE_DOWN])
		move_text_y(-0.05);
	if (keystate[SDL_SCANCODE_UP])
		move_text_y(0.05);
	if (mousewheel != 0)
	move_text_y(mousewheel * 30);
}

void ScrollableText::render(int x_pos, int y_pos)
{
	SDL_Rect viewport{x_pos, y_pos, _width, _height};
	SDL_RenderSetViewport(_renderer, &viewport);

	if (_texture != nullptr) {
		SDL_Rect dest{ _text_x, _text_y, _text_width, _text_height };
		SDL_RenderCopy(_renderer, _texture, NULL, &dest);
	}
}

void ScrollableText::set_text(std::string text)
{
	_text = text;
	get_texture(text);
	_text_x = _text_y = 0;
}

bool ScrollableText::get_texture(std::string text) {
	SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(_font, text.c_str(), { 0, 0, 0 }, _width);
	if (surface == nullptr)
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else {
		_texture = SDL_CreateTextureFromSurface(_renderer, surface);
		_text_width = surface->w;
		_text_height = surface->h;
		SDL_FreeSurface(surface);
		if (_texture == nullptr)
			printf("Unable to create texture: %s\n", SDL_GetError());
		else
			return true;
	}
	return false;
}

void ScrollableText::move_text_y(double y_diff)
{
	_text_y += y_diff;
	if (_text_y > 0)
		_text_y = 0;
	else if (_text_y < _height - _text_height)
		_text_y = _height - _text_height;		
}
