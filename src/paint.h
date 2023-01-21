#pragma once

#include <Windows.h>

// Generic Font family's
enum fontfamily {
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

void SetFont(HFONT& fontObject, const int iSize, const unsigned short usStyle, const fontfamily ffFamily, const wchar_t* cFontName);