#pragma once
#include <SDL.h>
#include <unordered_map>
#include <memory>

class Font
{
public:
	static std::shared_ptr<Font> get_font(const std::string filename, SDL_Renderer *renderer);
	Font(const std::string name, SDL_Renderer *renderer);
	virtual ~Font();
	class Glyph_info {
	public:
		SDL_Rect sourceRect;
		int xoffset, yoffset, xadvance;
	};
	std::vector<std::string> get_wrapped_lines(std::string &input, int width);
	int get_line_height();
	int get_xoffset(const char c);
	int get_yoffset(const char c);
	int get_xadvance(const char c);
	void set_colour(Uint8 r, Uint8 g, Uint8 b);
	void render_char(const char c, int x, int y);
private:
	static std::unordered_map<std::string, std::shared_ptr<Font>> _cache;	
	bool load_texture();
	bool load_info(const std::string filename);
	int get_next_value(std::string &line, std::string::const_iterator &it);
	std::string get_next_string(std::string &line, std::string::const_iterator &it);
	std::unordered_map<char, Glyph_info> _glyph_info;
	int _line_height;
	SDL_Renderer *_renderer;
	std::string _image_filename;
	SDL_Texture *_texture;
};

