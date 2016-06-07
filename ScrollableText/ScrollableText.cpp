#include "IWidget.h"
#include "ScrollableText.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Utils.h"

ScrollableText::ScrollableText(SDL_Renderer *renderer, const std::string font, int width, int height) : IWidget(width, height)
{
	_renderer = renderer;
	_texture = nullptr;
	_font = nullptr;
	load_font_texture(font + ".png");
	_font_coords = utils::get_glyph_coords(font + ".fnt");
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
	SDL_Rect source = _font_coords['C'];
	_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, source.w, source.h);
	SDL_SetRenderTarget(_renderer, _texture);

	SDL_Rect dest = { 0,0,source.w, source.h };
	SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(_renderer, &dest);
	SDL_RenderCopy(_renderer, _font, &source, &dest);
	_text_width = source.w; _text_height = source.h;

	SDL_SetRenderTarget(_renderer, nullptr);
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

bool ScrollableText::load_font_texture(const std::string filename)
{
	SDL_Surface *surface = IMG_Load(filename.c_str());
	if (surface == nullptr) {
		printf("Load image error: %s\n", IMG_GetError());
	}
	else {		
		//SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));		
		_font = SDL_CreateTextureFromSurface(_renderer, surface);		
		SDL_FreeSurface(surface);
		if (_font == nullptr) {
			printf("Create texture from surface error: %s\n", SDL_GetError());
		}
		else {
			return true;
		}		
	}
	return false;
}
