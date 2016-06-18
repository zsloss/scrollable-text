#include "Font.h"
#include <SDL_image.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>


Font::Font(const std::string name, SDL_Renderer *renderer)
{
	_renderer = renderer;
	load_info(name + ".fnt");
	load_texture();
}


Font::~Font()
{
	SDL_DestroyTexture(_texture);
	_texture = nullptr;
	_renderer = nullptr;
}

std::shared_ptr<Font> Font::get_font(const std::string name, SDL_Renderer *renderer) {
	if (Font::_cache.find(name) != _cache.end()) {
		return _cache[name];
	}
	auto font = std::make_shared<Font>(name, renderer);
	_cache[name] = font;
	return font;
}

bool Font::load_texture()
{
	SDL_Surface *surface = IMG_Load(_image_filename.c_str());
	if (surface == nullptr) {
		printf("Load image error: %s\n", IMG_GetError());
	}
	else {
		_texture = SDL_CreateTextureFromSurface(_renderer, surface);
		SDL_FreeSurface(surface);
		if (_texture == nullptr) {
			printf("Create texture from surface error: %s\n", SDL_GetError());
		}
		else {
			return true;
		}
	}
	return false;
}

bool Font::load_info(const std::string filename) {

	std::string line;
	std::ifstream file(filename.c_str());

	if (file.is_open()) {
		while (std::getline(file, line)) {

			if (line.substr(0, 2) == "co") { // "common"
				auto it = line.cbegin();
				_line_height = get_next_value(line, it);
			}

			else if (line.substr(0, 4) == "page") {
				auto it = line.cbegin();
				auto page_id = get_next_value(line, it);
				_image_filename = get_next_string(line, it);
			}

			else if (line.substr(0, 5) == "char ") {
				auto it = line.cbegin();

				char glyph = static_cast<char>(get_next_value(line, it));

				_glyph_info[glyph].sourceRect = { get_next_value(line, it), get_next_value(line, it), get_next_value(line, it), get_next_value(line, it) };
				_glyph_info[glyph].xoffset = get_next_value(line, it);
				_glyph_info[glyph].yoffset = get_next_value(line, it);
				_glyph_info[glyph].xadvance = get_next_value(line, it);
			}
		}
	}

	return true;
}

int Font::get_next_value(std::string &line, std::string::const_iterator &it) {

	// Move to the next '='
	while (it != line.cend() && *it != '=') {
		++it;
	}

	if (it == line.cend()) {
		throw "Iterator past end of line";
	}

	// Move to the integer value
	++it;

	std::ostringstream ss;
	while (it != line.cend() && *it != ' ') {
		ss << *(it++);
	}

	return stoi(ss.str());
}

std::string Font::get_next_string(std::string &line, std::string::const_iterator &it) {

	// Move to the next '='
	while (it != line.cend() && *it != '=') {
		++it;
	}

	if (it == line.cend()) {
		throw "Iterator past end of line";
	}

	// Move to the integer value
	it += 2;

	std::ostringstream ss;
	while (it != line.cend() && *it != '"') {
		ss << *(it++);
	}

	return ss.str();
}

std::vector<std::string> Font::get_wrapped_lines(std::string &input, int width) {
	std::vector<std::string> lines;
	int x = 0;
	std::string::const_iterator end_of_word, start_of_line, undefined = input.cend();
	for (auto it = start_of_line = input.cbegin(); it != input.cend(); ++it) {

		if (*it == '\n') {
			lines.push_back(std::string(start_of_line, it));
			start_of_line = it + 1;
			end_of_word = undefined;
			x = _glyph_info[*it].xadvance;
		}
		else if (*it == ' ') {
			end_of_word = it;
			x += _glyph_info[*it].xadvance;
		}
		else if (*it == '<' || *it == '>') {
		}
		else {
			x += _glyph_info[*it].xadvance;
			if (x >= width) {
				if (end_of_word != undefined) {
					lines.push_back(std::string(start_of_line, end_of_word));
					it = start_of_line = end_of_word + 1;
					x = _glyph_info[*it].xadvance;
				}
				else {
					lines.push_back(std::string(start_of_line, it));
					start_of_line = it;
					x = _glyph_info[*it].xadvance;
				}
				end_of_word = undefined;
			}
		}
	}
	lines.push_back(std::string(start_of_line, input.cend()));

	return lines;
}

int Font::get_line_height()
{
	return _line_height;
}

int Font::get_xoffset(const char c)
{
	return _glyph_info[c].xoffset;
}

int Font::get_yoffset(const char c)
{
	return _glyph_info[c].yoffset;
}

int Font::get_xadvance(const char c)
{
	return _glyph_info[c].xadvance;
}

void Font::set_colour(Uint8 r, Uint8 g, Uint8 b)
{
	SDL_SetTextureColorMod(_texture, r, g, b);
}

void Font::render_char(const char c, int x, int y)
{
	auto source = &_glyph_info[c].sourceRect;
	auto dest = SDL_Rect{ x + get_xoffset(c), y + get_yoffset(c), source->w, source->h };
	SDL_RenderCopy(_renderer, _texture, source, &dest);
}


std::unordered_map<std::string, std::shared_ptr<Font>> Font::_cache;
