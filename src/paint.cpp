#include "paint.h"

void SetFont(HFONT& fontObject, const int iSize, const unsigned short usStyle, const fontfamily ffFamily, const wchar_t* cFontName) {
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

	fontObject = CreateFontIndirect(&lf);
}