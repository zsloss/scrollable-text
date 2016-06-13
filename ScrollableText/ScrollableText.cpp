#include "IWidget.h"
#include "ScrollableText.h"
#include <SDL.h>
#include <SDL_image.h>
#include "Utils.h"

ScrollableText::ScrollableText(SDL_Renderer *renderer, const std::string font, int x_pos, int y_pos, int width, int height) : IWidget(x_pos, y_pos, width, height)
{
	_renderer = renderer;
	_texture = nullptr;
	_font = nullptr;
	load_font_texture(font + ".png");
	_font_info = utils::get_font_info(font + ".fnt");
}

ScrollableText::~ScrollableText()
{
	SDL_DestroyTexture(_texture);
	_texture = nullptr;
}

void ScrollableText::render()
{
	SDL_Rect viewport{_x, _y, _width, _height};
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

	auto lines = utils::get_wrapped_lines(text, _width, _font_info);

	auto line_height = _font_info.line_height;
	_text_height = lines.size() * line_height;
	_text_width = _width;

	_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _text_width, _text_height);

	if (_texture != nullptr) {
		// Prepare the texture
		SDL_SetRenderTarget(_renderer, _texture);
		SDL_SetRenderDrawColor(_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_Rect whole = { 0,0, _text_width, _text_height };
		SDL_RenderFillRect(_renderer, &whole);

		int xPos = 0, yPos = 0;
		for (auto lines_it = lines.cbegin(); lines_it != lines.cend(); ++lines_it) {
			auto line = *lines_it;
			bool in_link = false;
			SDL_Rect temp_link;
			SDL_SetTextureColorMod(_font, 0, 0, 0);
			for (auto string_it = line.cbegin(); string_it != line.cend(); ++string_it) {

				if (*string_it == '<') {
					in_link = true;
					SDL_SetTextureColorMod(_font, 0xFF, 0, 0);
					temp_link = { xPos, yPos, 0, line_height };
				}
				else if (*string_it == '>') {
					if (in_link) {
						_links.push_back(temp_link);
						SDL_SetTextureColorMod(_font, 0, 0, 0);
					}
					in_link = false;
				}
				else {
					auto info = _font_info.glyph_info[*string_it];

					SDL_Rect source = info.sourceRect;
					SDL_Rect dest = { xPos + info.xoffset, yPos + info.yoffset, source.w, source.h };

					SDL_RenderCopy(_renderer, _font, &source, &dest);
					xPos += info.xadvance;

					if (in_link)
						temp_link.w += info.xadvance;
				}
			}

			xPos = 0; yPos += line_height;
		}

		SDL_SetRenderTarget(_renderer, nullptr);
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

bool ScrollableText::load_font_texture(const std::string filename)
{
	SDL_Surface *surface = IMG_Load(filename.c_str());
	if (surface == nullptr) {
		printf("Load image error: %s\n", IMG_GetError());
	}
	else {			
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

void ScrollableText::scroll_up() {
	move_text_y(30);
}

void ScrollableText::scroll_down() {
	move_text_y(-30);
}

void ScrollableText::click(int x, int y) {
	x -= _x;
	y -= _y;

	for (auto it = _links.cbegin(); it != _links.cend(); ++it) {
		auto link = *it;
		if (x >= link.x && x <= link.x + link.w && y >= link.y + _text_y && y <= link.y + _text_y + link.h) {
			printf("Link clicked!\n");
		}
	}
}
