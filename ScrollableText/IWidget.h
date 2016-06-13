#pragma once
class IWidget
{
public:	
	virtual ~IWidget() {}
	virtual void update() {}
	virtual void render() = 0;
	virtual int get_width() { return _width; }
	virtual int get_height() { return _height; }
	virtual int get_x() { return _x; }
	virtual int get_y() { return _y; }
	virtual void scroll_up() {}
	virtual void scroll_down() {}
	virtual void click(int x, int y) {}
protected:
	IWidget(int x_pos, int y_pos, int width, int height) : _x(x_pos), _y(y_pos), _width(width), _height(height) {}
	int _x, _y, _width, _height;
};
