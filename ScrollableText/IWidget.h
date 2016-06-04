#pragma once
class IWidget
{
public:	
	virtual ~IWidget() {}
	virtual void update() {};
	virtual void render(int x_pos, int y_pos) = 0;
	virtual int get_width() { return _width; }
	virtual int get_height() { return _height; }
protected:
	IWidget(int width, int height) : _width(width), _height(height) {}
	int _width, _height;
};
