#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <SDL.h>
#include "Utils.h"
#include <sstream>

namespace utils {	
	
	int get_next_value(std::string &line, std::string::const_iterator &it) {

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
			ss << *it++;
		}

		return stoi(ss.str());
	}

	Font_info get_font_info(const std::string filename) {

		static std::unordered_map<std::string, Font_info> cache;
		if (cache.find(filename) != cache.end()) {
			return cache[filename];
		}

		std::string line;
		std::ifstream file(filename.c_str());

		Font_info font_info;
		if (file.is_open()) {
			while (std::getline(file, line)) {
				
				if (line.substr(0, 2) == "co") { // "common"
					auto it = line.cbegin();
					font_info.line_height = get_next_value(line, it);
				}

				if (line.substr(0, 5) == "char ") {
					auto it = line.cbegin();					

					char glyph = static_cast<char>(get_next_value(line, it));

					font_info.glyph_info[glyph].sourceRect = { get_next_value(line, it), get_next_value(line, it), get_next_value(line, it), get_next_value(line, it) };
					font_info.glyph_info[glyph].xoffset = get_next_value(line, it);
					font_info.glyph_info[glyph].yoffset = get_next_value(line, it);
					font_info.glyph_info[glyph].xadvance = get_next_value(line, it);
				}
			}
		}

		cache[filename] = font_info;
		return font_info;
	}

}
