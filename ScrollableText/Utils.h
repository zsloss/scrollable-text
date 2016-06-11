#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <SDL.h>
#include "Utils.h"

namespace utils {

	class Font_info {
	public:
		class Glyph_info {
		public:
			SDL_Rect sourceRect;
			int xoffset, yoffset, xadvance;
		};
		int line_height;
		std::unordered_map<char, Glyph_info> glyph_info;
	};

	std::vector<std::string> split(std::string input, char delimiter);
	std::vector<std::string> get_wrapped_lines(std::string &input, int width, Font_info &font_info);	

	Font_info get_font_info(const std::string filename);
}