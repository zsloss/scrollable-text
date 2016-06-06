#pragma once
#include <unordered_map>
#include <SDL.h>

namespace utils {

	std::unordered_map<char, SDL_Rect> get_glyph_coords(const char* filename);

}