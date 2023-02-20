#pragma once

#include "Component.h"

class Text: public Component
{
private:
	std::string text;
	paint::Font font;
	LONG maxWidth;
	LONG maxHeight;
	COLORREF color;
	UINT style;
public:
	Text(RECT rect, std::string_view text, HWND hwnd, COLORREF color, const paint::Font& font, UINT style = BS_CENTER);
	void paint(HDC hdc) const override;
	void SetText(std::string_view text);
	std::string GetText() const;
	void SetFont(const paint::Font& font);
	paint::Font GetFont() const;
	void SetColor(COLORREF color);
	COLORREF GetColor() const;
	void SetStyle(UINT style);
	UINT GetStyle() const;
	nlohmann::json Serialize() const override;
	static std::shared_ptr<Text> Deserialize(const nlohmann::json& serializedText, HWND hwnd);
};
