#pragma once
#include <wtypes.h>
#include <string>

#include "../paint.h"

class Component
{
protected:
	RECT rect;
	int id;
	paint::Pen pen;
	paint::Brush brush;
	static int idCounter;
public:
	explicit Component(RECT rect, const paint::Pen& pen = paint::Pen(PS_SOLID, 1, RGB(0, 0, 0)), const paint::Brush& brush = paint::Brush(RGB(0, 0, 0)));
	virtual ~Component();
	virtual void paint(HDC hdc) const = 0;
	void SetPen(const paint::Pen& pen);
	void SetBrush(const paint::Brush& brush);
	virtual void move(const POINT& point);
	virtual void rmove(const POINT& delta);
	bool IsPointInComponent(const POINT& point) const;
	paint::Pen GetPen() const;
	paint::Brush GetBrush() const;
};
