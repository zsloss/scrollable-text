#include "Widget.h"
#include "ScrollableText.h"
#include <SDL.h>
#include "Utils.h"

ScrollableText::ScrollableText(SDL_Renderer *renderer, const std::string font, int x_pos, int y_pos, int width, int height) 
	: Widget(x_pos, y_pos, width, height), _renderer(renderer), _texture(nullptr), _font(Font::get_font(font, renderer)), _scrollable(false)
{
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

	auto lines = _font->get_wrapped_lines(text, _width);

	auto line_height = _font->get_line_height();
	_text_height = lines.size() * line_height;
	_text_width = _width;

	_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _text_width, _text_height);

	if (_texture != nullptr) {
		// Prepare the texture
		SDL_SetRenderTarget(_renderer, _texture);
		SDL_SetRenderDrawColor(_renderer, 0,0,0, 0xFF);
		SDL_Rect whole = { 0,0, _text_width, _text_height };
		SDL_RenderFillRect(_renderer, &whole);

		int xPos = 0, yPos = 0;
		for (auto lines_it = lines.cbegin(); lines_it != lines.cend(); ++lines_it) {
			auto line = *lines_it;
			bool in_link = false;
			SDL_Rect temp_link;
			_font->set_colour(0xFF, 0xFF, 0xFF);
			for (auto string_it = line.cbegin(); string_it != line.cend(); ++string_it) {

				if (*string_it == '<') {
					in_link = true;
					_font->set_colour(0xFF, 0, 0);
					temp_link = { xPos, yPos, 0, line_height };
				}
				else if (*string_it == '>') {
					if (in_link) {
						_links.push_back(temp_link);
						_font->set_colour(0xFF, 0xFF, 0xFF);
					}
					in_link = false;
				}
				else {
					_font->render_char(*string_it, xPos, yPos);
					xPos += _font->get_xadvance(*string_it);

					if (in_link)
						temp_link.w += _font->get_xadvance(*string_it);
				}
			}

			xPos = 0; yPos += line_height;
		}

		_scrollable = _text_height > _height; // Only scrollable if needed.
		SDL_SetRenderTarget(_renderer, nullptr);
		return true;
	}

	return false;
}

void ScrollableText::move_text_y(double y_diff)
{
	if (_scrollable) {
		_text_y += y_diff;
		if (_text_y > 0)
			_text_y = 0;
		else if (_text_y < _height - _text_height)
			_text_y = _height - _text_height;
	}
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
