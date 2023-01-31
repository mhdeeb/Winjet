#pragma once

#include "Component.h"

class Text : public Component
{
private:
	std::wstring text;
	paint::Font font;
	COLORREF color;
	UINT style;
public:
	Text(RECT rect, std::wstring_view text, HWND hwnd = nullptr, COLORREF color = RGB(0, 0, 0), const paint::Font& font = { 40, BOLD, MODERN, L"Curier New" }, UINT style = BS_CENTER);
	void paint(HDC hdc) const override;
	void SetText(std::wstring_view text);
	std::wstring GetText() const;
	void SetFont(const paint::Font& font);
	paint::Font GetFont() const;
	void SetColor(COLORREF color);
	COLORREF GetColor() const;
	void SetStyle(UINT style);
	UINT GetStyle() const;
	//FIX
	nlohmann::json Serialize() const override;
	//FIX
	static std::shared_ptr<Text> Deserialize(const nlohmann::json& serializedText);
};
