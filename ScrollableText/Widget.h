#pragma once
#include <SDL.h>

class Widget
{
public:	
	virtual ~Widget() {}
	virtual void update() {}
	virtual void render() = 0;
	virtual int get_width() { return _width; }
	virtual int get_height() { return _height; }
	virtual int get_x() { return _x; }
	virtual int get_y() { return _y; }
	virtual void set_background_colour(Uint8 r, Uint8 g, Uint8 b) { background_colour.r = r; background_colour.g = g; background_colour.b = b; }
	virtual void set_foreground_colour(Uint8 r, Uint8 g, Uint8 b) { foreground_colour.r = r; foreground_colour.g = g; foreground_colour.b = b; }
	virtual void scroll_up() {}
	virtual void scroll_down() {}
	virtual void click(int x, int y) {}
protected:
	Widget(int x_pos, int y_pos, int width, int height) : _x(x_pos), _y(y_pos), _width(width), _height(height) {}
	int _x, _y, _width, _height;
	struct Colour {
		Uint8 r, g, b;
	};
	Colour background_colour, foreground_colour;
};
