#include "Text.h"

Text::Text(RECT rect, std::string_view text, COLORREF color, const paint::Font& font, UINT style): Component(rect), text(text), font(font), color(color), style(style) {}

void Text::paint(HDC hdc) const {
	std::wstring wcommand(text.begin(), text.end());
	const TCHAR* szBuffer = wcommand.c_str();
	RECT rc(rect);
	SelectObject(hdc, font.GetFont());
	SetTextColor(hdc, color);
	DrawText(hdc, szBuffer, int(wcommand.size()), &rc, BS_CENTER);
}

void Text::SetText(std::string_view text) {
	this->text = text;
}

std::string Text::GetText() const {
	return text;
}

void Text::SetFont(const paint::Font& font) {
	this->font = font;
}

paint::Font Text::GetFont() const {
	return font;
}

void Text::SetColor(COLORREF color) {
	this->color = color;
}

COLORREF Text::GetColor() const {
	return color;
}