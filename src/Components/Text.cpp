#include "Text.h"

#include "../includes/json.hpp"

Text::Text(RECT rect, std::wstring_view text, HWND hwnd, COLORREF color, const paint::Font& font, UINT style) : Component(rect, hwnd), text(text), font(font), color(color), style(style) {}

void Text::paint(HDC hdc) const {
	const TCHAR* szBuffer = text.c_str();
	RECT rc(rect);
	SelectObject(hdc, font.GetFont());
	SetTextColor(hdc, color);
	DrawText(hdc, szBuffer, int(text.size()), &rc, BS_CENTER);
}

void Text::SetText(std::wstring_view text) {
	this->text = text;
	Invalidate();
}

std::wstring Text::GetText() const {
	return text;
}

void Text::SetFont(const paint::Font& font) {
	this->font = font;
	Invalidate();
}

paint::Font Text::GetFont() const {
	return font;
}

void Text::SetColor(COLORREF color) {
	this->color = color;
	Invalidate();
}

COLORREF Text::GetColor() const {
	return color;
}

void Text::SetStyle(UINT style) {
	this->style = style;
	Invalidate();
}

UINT Text::GetStyle() const {
	return style;
}

//FIX
nlohmann::json Text::Serialize() const {
	nlohmann::json j;
	j["Component"] = "Text";
	j["Rect"] = { rect.left, rect.top, rect.right, rect.bottom };
	j["Text"] = text;
	j["Font"] = font.Serialize();
	j["Color"] = RGBToHex(color);
	j["Style"] = style;
	return j;
}

//FIX
std::shared_ptr<Text> Text::Deserialize(const nlohmann::json& serializedText) {
	auto rect = serializedText["Rect"].get<std::vector<int>>();
	auto text = serializedText["Text"].get<std::string>();
	auto font = paint::Font::Deserialize(serializedText["Font"]);
	auto colortxt = serializedText["Color"].get<std::string>();
	auto color = HexToRGB(std::wstring(colortxt.begin(), colortxt.end()));
	auto style = serializedText["Style"].get<UINT>();
	return std::make_shared<Text>(RECT{ rect[0], rect[1], rect[2], rect[3] }, std::wstring(text.begin(), text.end()), nullptr, color, font, style);
}