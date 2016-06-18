#include <unordered_map>
#include <SDL.h>
#include "Utils.h"
#include <sstream>

namespace utils {

	std::vector<std::string> utils::split(std::string input, char delimiter) {
		std::vector<std::string> ret;
		std::istringstream ss(input);
		std::string elem;
		while (std::getline(ss, elem, delimiter)) {
			ret.push_back(elem);
		}

		return ret;
	}
}