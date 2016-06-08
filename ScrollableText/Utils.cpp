#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>
#include <SDL.h>
#include "Utils.h"

namespace utils {

	Font_info get_font_info(const std::string filename) {
		std::string line;
		std::ifstream file(filename.c_str());

		Font_info font_info;
		if (file.is_open()) {
			while (std::getline(file, line)) {
				std::smatch match;
				if (std::regex_search(line, match, std::regex(R"(^char\s+id=(-?\d+)\s+x=(-?\d+)\s+y=(-?\d+)\s+width=(-?\d+)\s+height=(-?\d+)\s+xoffset=(-?\d+)\s+yoffset=(-?\d+)\s+xadvance=(-?\d+))"))) {

					char glyph = static_cast<char>(stoi(match[1]));
					int x = stoi(match[2]), y = stoi(match[3]),
						w = stoi(match[4]), h = stoi(match[5]),
						xoffset = stoi(match[6]), yoffset = stoi(match[7]),
						xadvance = stoi(match[8]);

					font_info.glyph_info[glyph].sourceRect = { x, y, w, h };
					font_info.glyph_info[glyph].xoffset = xoffset;
					font_info.glyph_info[glyph].yoffset = yoffset;
					font_info.glyph_info[glyph].xadvance = xadvance;
				}
			}
		}

		return font_info;
	}

}
