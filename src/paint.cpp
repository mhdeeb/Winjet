#include "paint.h"

using namespace paint;

Font::Font(int fontSize, unsigned short fontStyle, fontfamily fontFamily, const wchar_t* fontName) { SetFont(fontSize, fontStyle, fontFamily, fontName); }

Font::Font(const paint::Font& font) {
	SetFont(font.fontSize, font.fontStyle, font.fontFamily, font.fontName);
}

Font::~Font() { DeleteObject(font); }

void Font::SetFont(const int iSize, const unsigned short usStyle, const fontfamily ffFamily, const wchar_t* cFontName) {
	DeleteObject(font);

	fontSize = iSize;
	fontStyle = usStyle;
	fontFamily = ffFamily;
	fontName = cFontName;

	LOGFONT lf{sizeof(lf)};

	lf.lfHeight = iSize;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;

	if ((usStyle & BOLD) == BOLD) {
		lf.lfWeight = FW_BOLD;
	} else {
		lf.lfWeight = FW_NORMAL;
	}

	if ((usStyle & ITALICIZED) == ITALICIZED) {
		lf.lfItalic = true;
	} else {
		lf.lfItalic = false;
	}

	if ((usStyle & UNDERLINED) == UNDERLINED) {
		lf.lfUnderline = true;
	} else {
		lf.lfUnderline = false;
	}

	if ((usStyle & STRIKEOUT) == STRIKEOUT) {
		lf.lfStrikeOut = true;
	} else {
		lf.lfStrikeOut = false;
	}

	lf.lfStrikeOut = FALSE;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;

	switch (ffFamily) {
	case MODERN:
		lf.lfPitchAndFamily = FF_MODERN;
		break;

	case ROMAN:
		lf.lfPitchAndFamily = FF_ROMAN;
		break;

	case SCRIPT:
		lf.lfPitchAndFamily = FF_SCRIPT;
		break;

	case SWISS:
		lf.lfPitchAndFamily = FF_SWISS;
		break;

	default:
		lf.lfPitchAndFamily = FF_DONTCARE;
		break;
	}

	lf.lfPitchAndFamily |= DEFAULT_PITCH;
	if (cFontName)
		wcscpy_s(lf.lfFaceName, cFontName);

	font = CreateFontIndirect(&lf);
}

HFONT Font::GetFont() const { return font; }

Brush::Brush(COLORREF brushColor) { SetBrush(brushColor); }

Brush::Brush(const paint::Brush& brush) {
	SetBrush(brush.brushColor);
}

Brush::~Brush() { DeleteObject(brush); }

void Brush::SetBrush(COLORREF brushColor) {
	DeleteObject(brush);

	this->brushColor = brushColor;

	brush = CreateSolidBrush(brushColor);
}

HBRUSH Brush::GetBrush() const { return brush; }

Pen::Pen(int penStyle, int penWidth, COLORREF penColor) { SetPen(penStyle, penWidth, penColor); }

Pen::Pen(const paint::Pen& pen) {
	SetPen(pen.penStyle, pen.penWidth, pen.penColor);
}

Pen::~Pen() { DeleteObject(pen); }

void Pen::SetPen(int penStyle, int penWidth, COLORREF penColor) {
	DeleteObject(pen);

	this->penStyle = penStyle;
	this->penWidth = penWidth;
	this->penColor = penColor;

	pen = CreatePen(penStyle, penWidth, penColor);
}

HPEN Pen::GetPen() const { return pen; }