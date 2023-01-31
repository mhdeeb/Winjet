#pragma once
#include <wtypes.h>
#include <string>
#include <memory>

#include "../paint.h"

class Component : public Serializable {
protected:
	RECT rect;
	int id;
	paint::Pen pen;
	paint::Brush brush;
	static int idCounter;
	HWND hwnd;
public:
	explicit Component(RECT rect, HWND hwnd = nullptr, const paint::Pen& pen = paint::Pen(PS_SOLID, 1, RGB(0, 0, 0)), const paint::Brush& brush = paint::Brush(RGB(0, 0, 0)));
	~Component() override;
	virtual void paint(HDC hdc) const = 0;
	void Invalidate() const;
	void SetPen(const paint::Pen& pen);
	void SetBrush(const paint::Brush& brush);
	virtual void move(const POINT& point);
	virtual void rmove(const POINT& delta);
	bool IsPointInComponent(const POINT& point) const;
	paint::Pen GetPen() const;
	paint::Brush GetBrush() const;
	//FIX
	nlohmann::json Serialize() const override = 0;
	virtual void SetHwnd(HWND hwnd);
	HWND GetHwnd() const;
	void SetRect(RECT rect);
	RECT GetRect() const;
	//FIX
	static std::shared_ptr<Component>  Deserialize(const nlohmann::json& data);
};
