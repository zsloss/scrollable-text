#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>
#include <SDL.h>

namespace utils {

	std::unordered_map<char, SDL_Rect> get_glyph_coords(const char* filename) {
		std::string line;
		std::ifstream file(filename);

		std::unordered_map<char, SDL_Rect> char_map;
		if (file.is_open()) {
			while (std::getline(file, line)) {
				std::smatch match;
				if (std::regex_search(line, match, std::regex(R"(^char\s+id=(\d+)\s+x=(\d+)\s+y=(\d+)\s+width=(\d+)\s+height=(\d+))"))) {

					char glyph = static_cast<char>(stoi(match[1]));
					int x = stoi(match[2]), y = stoi(match[3]),
						w = stoi(match[4]), h = stoi(match[5]);

					char_map[glyph] = { x, y, w, h };
				}
			}

			file.close();
		}

		return char_map;
	}

}