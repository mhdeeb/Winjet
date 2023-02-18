#pragma once

#include <Windows.h>
#include <string>

#include "util.h"

enum class fontfamily {
	BY_NAME,	// use font name instead of abstract type
	MODERN,		// monospace with or without serifs
	ROMAN,		// variable width with serifs
	SCRIPT,		// like handwriting
	SWISS		// variable width without serifs
};

const unsigned char PLAIN = 0x00;
const unsigned char BOLD = 0x01;
const unsigned char ITALICIZED = 0x02;
const unsigned char UNDERLINED = 0x04;
const unsigned char STRIKEOUT = 0x08;

namespace paint {
	class Font: public Serializable {
	private:
		HFONT font;
		int fontSize;
		unsigned short fontStyle;
		fontfamily fontFamily;
		std::string fontName;
	public:
		Font(int fontSize, unsigned short fontStyle, fontfamily fontFamily, std::string_view fontName);
		Font(const paint::Font& font);
		~Font() override;
		void SetFont(int fontSize, unsigned short fontStyle, fontfamily fontFamily, std::string_view fontName);
		HFONT GetFont() const;
		int GetFontSize() const;
		unsigned short GetFontStyle() const;
		fontfamily GetFontFamily() const;
		std::string GetFontName() const;
		void SetFontSize(int fontSize);
		void SetFontStyle(unsigned short fontStyle);
		void SetFontFamily(fontfamily fontFamily);
		void SetFontName(const char* fontName);
		nlohmann::json Serialize() const override;
		static Font Deserialize(const nlohmann::json& serializedFont);
	};

	class Brush: public Serializable {
	private:
		HBRUSH brush;
		COLORREF brushColor;
	public:
		explicit Brush(COLORREF brushColor = RGB(0, 0, 0));
		Brush(const paint::Brush& brush);
		~Brush() override;
		void SetBrush(COLORREF brushColor = RGB(0, 0, 0));
		HBRUSH GetBrush() const;
		void SetBrushColor(COLORREF brushColor);
		COLORREF GetBrushColor() const;
		nlohmann::json Serialize() const override;
		static Brush Deserialize(const nlohmann::json& serializedBrush);
	};

	class Pen: public Serializable {
	private:
		HPEN pen;
		int penStyle;
		int penWidth;
		COLORREF penColor;
	public:
		Pen(int penStyle = PS_SOLID, int penWidth = 1, COLORREF penColor = RGB(0, 0, 0));
		Pen(const paint::Pen& pen);
		~Pen() override;
		void SetPen(int penStyle = PS_SOLID, int penWidth = 1, COLORREF penColor = RGB(0, 0, 0));
		HPEN GetPen() const;
		int GetPenStyle() const;
		int GetPenWidth() const;
		COLORREF GetPenColor() const;
		void SetPenStyle(int penStyle = PS_SOLID);
		void SetPenWidth(int penWidth = 1);
		void SetPenColor(COLORREF penColor = RGB(0, 0, 0));
		nlohmann::json Serialize() const override;
		static Pen Deserialize(const nlohmann::json& serializedPen);
	};

	enum Color {
		TRANSPARENTC = RGB(0, 1, 0),
		WHITE = RGB(255, 255, 255),
		BLACK = RGB(0, 0, 0),
		RED = RGB(255, 0, 0),
		GREEN = RGB(0, 255, 0),
		BLUE = RGB(0, 0, 255),
		YELLOW = RGB(255, 255, 0),
		CYAN = RGB(0, 255, 255),
		MAGENTA = RGB(255, 0, 255),
		GRAY = RGB(128, 128, 128),
		LIGHT_GRAY = RGB(192, 192, 192),
		DARK_GRAY = RGB(64, 64, 64),
		ORANGE = RGB(255, 200, 0),
		PINK = RGB(255, 175, 175),
		BROWN = RGB(150, 75, 0),
		LIGHT_GREEN = RGB(175, 255, 175),
		LIGHT_BLUE = RGB(175, 175, 255),
		LIGHT_YELLOW = RGB(255, 255, 175),
		LIGHT_CYAN = RGB(175, 255, 255),
		LIGHT_MAGENTA = RGB(255, 175, 255),
		LIGHT_ORANGE = RGB(255, 225, 175),
		LIGHT_PINK = RGB(255, 200, 200),
		LIGHT_BROWN = RGB(200, 150, 100),
		DARK_GREEN = RGB(0, 128, 0),
		DARK_BLUE = RGB(0, 0, 128),
		DARK_YELLOW = RGB(128, 128, 0),
		DARK_CYAN = RGB(0, 128, 128),
		DARK_MAGENTA = RGB(128, 0, 128),
		DARK_ORANGE = RGB(128, 64, 0),
		DARK_PINK = RGB(128, 0, 0),
		DARK_BROWN = RGB(64, 32, 0),
	};
}
