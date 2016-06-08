#pragma once
#include <unordered_map>
#include <SDL.h>

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

	Font_info get_font_info(const std::string filename);

}