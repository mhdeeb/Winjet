﻿#include "Text.h"

#include "../includes/json.hpp"
#include <iostream>

Text::Text(RECT rect, std::string_view text, HWND hwnd, COLORREF color, const paint::Font& font, UINT style):
	Component(rect, hwnd, paint::Pen(), paint::Brush()),
	maxWidth(rect.right - rect.left), maxHeight(rect.bottom - rect.top),
	font(font), color(color), style(style) {
	SetText(text);
}

void Text::paint(HDC hdc) const {
	RECT rc(rect);
	SelectObject(hdc, font.GetFont());
	SetTextColor(hdc, color);
	std::wstring wtext = to_utf8(text);
	DrawText(hdc, wtext.c_str(), int(wtext.size()), &rc, BS_CENTER | DT_WORDBREAK);
}

void Text::SetText(std::string_view text) {
	rect.right = rect.left + font.GetFontSize() * to_utf8(text).size();
	rect.bottom = rect.top + font.GetFontSize() * 2;
	if (rect.right - rect.left > maxWidth)
		rect.right = rect.left + maxWidth;
	if (rect.bottom - rect.top > maxHeight)
		rect.bottom = rect.top + maxHeight;
	std::cout << text << ' ' << font.GetFontSize() * to_utf8(text).size() << ' ' << font.GetFontSize() * 2 << std::endl;
	Sleep(100);
	this->text = text;
	Invalidate();
}

std::string Text::GetText() const {
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

nlohmann::json Text::Serialize() const {
	nlohmann::json j;
	j["Component"] = "Text";
	j["Rect"] = { rect.left, rect.top, maxWidth, maxHeight };
	j["Text"] = text;
	j["Font"] = font.Serialize();
	j["Color"] = RGBToHex(color);
	j["Style"] = style;
	return j;
}

std::shared_ptr<Text> Text::Deserialize(const nlohmann::json& serializedText, HWND hwnd) {
	auto rect = serializedText["Rect"].get<std::vector<int>>();
	auto& text = serializedText["Text"];
	auto font = paint::Font::Deserialize(serializedText["Font"]);
	auto color = HexToRGB(serializedText["Color"].get<std::string>());
	auto style = serializedText["Style"].get<UINT>();
	return std::make_shared<Text>(RECT{ rect[0], rect[1], rect[0] + rect[2], rect[1] + rect[3] }, text, hwnd, color, font, style);
}