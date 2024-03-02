#include <SADXModLoader.h>
#include "Common.h"
#include <d3d8.h>
#include <d3dx8.h>
#include "FunctionHook.h"

DataPointer(IDirect3DDevice8*, Direct3D_Device, 0x03D128B0);
HFONT c_hFont;
ID3DXFont* font;
RECT fRectangle;
bool FontLoaded = false;
float FontScale = 1.0f;
DataPointer(MSGC, jimakumsgc, 0x03ABDC18);
char SubtitleString[256];
FunctionPointer(void, MSG_PutsCenter, (MSGC* msgc, char* str), 0x0040DDA0);
FunctionPointer(void, MSG_PutsBlockCenter, (MSGC* msgc, char* str), 0x0040DEB0);
//FunctionPointer(void, MSG_Cls, (MSGC* msgc), 0x0040D850);
FunctionPointer(void, MSG_LF, (MSGC* msgc), 0x40D9D0);
enum SubtitleAlignment : char
{
	SubtitleAlignLeft = 0, // Align all lines to the left
	SubtitleAlignCenter = 0x09, // Align all lines to the center
	SubtitleAlignCenterLongest = 0x07, // Center by the longest line
};

struct SubtitleLine
{
	char lineText[128];
	int lineNumChars;
};

struct SubtitleData
{
	SubtitleLine lines[4];
	int numLines;
	int numChars;
	SubtitleAlignment align;
	NJS_POINT2 rectPos;
	NJS_POINT2 rectSize;
	NJS_POINT2 textPos;
};

static SubtitleData Subtitles[32];
static int NumSubtitles = 0;

void AddSubtitleData(MSGC* msgc, char* str, SubtitleAlignment align)
{
	int id = NumSubtitles;
	int currentLine = 0;
	int currentLineChar = 0;
	for (int i = 0;i < 256;i++)
	{
		switch (str[i])
		{
			// End of string
		case 0:
			Subtitles[id].lines[currentLine].lineNumChars = currentLineChar;
			goto end;
			// Don't add alignment characters to character count
		case 0x07:
		case 0x09:
			break;
			// Line break
		case 0x0A:
			Subtitles[id].lines[currentLine].lineNumChars = currentLineChar;
			currentLineChar = 0;
			currentLine++;
			break;
		default:
			Subtitles[id].lines[currentLine].lineText[currentLineChar] = str[i];
			currentLineChar++;
			break;
		}
	}
end:
	Subtitles[id].numLines = currentLine + 1;
	for (int i = 0;i < Subtitles[id].numLines;i++)
	{
		PrintDebug("Subtitle %d line %d (%d chars): %s\n", id, i, Subtitles[id].lines[i].lineNumChars, Subtitles[id].lines[i].lineText);
	}
	Subtitles[id].align = align;
	Subtitles[id].rectPos.x = msgc->x;
	Subtitles[id].rectPos.y = msgc->y - 4;
	Subtitles[id].rectSize.x = msgc->width;
	Subtitles[id].rectSize.y = msgc->height + 4;
	Subtitles[id].textPos.x = msgc->lx;
	Subtitles[id].textPos.y = msgc->ly;
	NumSubtitles++;
}

int CalculateLongestLineSize()
{
	int linenumber = 0;
	int line1 = 0;
	int line2 = 0;
	int line3 = 0;
	int line4 = 0;
	int result = 0;
	int spaceadd = 0;
	for (unsigned int cursor = 0; cursor < 256; cursor++)
	{
		if (SubtitleString[cursor] == 0x0A)
		{
			linenumber++;
		}
		else if (SubtitleString[cursor] == 0)
		{
			result = max(max(line1, line2), max(line3, line4));
			goto results;
			break;
		}
		else if (SubtitleString[cursor] != 0x09 && SubtitleString[cursor] != 0x07)
		{
			switch (linenumber)
			{
			case 0:
				line1 += 1;
				break;
			case 1:
				line2 += 1;
				break;
			case 2:
				line3 += 1;
				break;
			case 3:
				line4 += 1;
				break;
			}
		}
	}
	goto results;
results:
	//PrintDebug("1: %d, 2: %d, 3: %d, 4: %d, longest line: %d\n", line1, line2, line3, line4, result);
	return (float)result;
}

int CalculateSubtitleCharacterCount(const char* str)
{
	// 0x09 centers all lines
	// 0x07 centers current line
	// Nothing centers all lines by the longest line
	int result = 0;
	for (int i = 0;i < 255;i++)
	{
		if (str[i] == 0)
			return result;
		result++;
	}
}

FunctionHook<void, MSGC*, char*> MSG_PutsCenter_h(MSG_PutsCenter);
FunctionHook<void, MSGC*, char*> MSG_PutsBlockCenter_h(MSG_PutsBlockCenter);
FunctionHook<void, MSGC*, const char*> MSG_Puts_h(MSG_Puts);
FunctionHook<void, MSGC*> MSG_Cls_h(MSG_Cls);

void MSG_Cls_r(MSGC* msgc)
{
	MSG_Cls_h.Original(msgc);
	for (int s = 0;s < NumSubtitles;s++)
	{

		for (int l = 0;l < Subtitles[s].numLines; l++)
		{
			for (int c = 0; c < Subtitles[s].lines[l].lineNumChars; c++)
			{
				Subtitles[s].lines[l].lineText[c] = 0;
			}
			Subtitles[s].lines[l].lineNumChars = 0;
		}
		Subtitles[s].numChars = 0;
		Subtitles[s].numLines = 0;
		Subtitles[s].align = SubtitleAlignLeft;
	}
	NumSubtitles = 0;
}

void MSG_PutsCenter_r(MSGC* msgc, char* str)
{
	MSG_PutsCenter_h.Original(msgc, str);
	PrintDebug("X: %X, Y: %d, W: %d, H:%d, LX: %d, LY: %d\n", msgc->x, msgc->y, msgc->width, msgc->height, msgc->lx, msgc->ly);
	AddSubtitleData(msgc, str, SubtitleAlignCenter);
}

void MSG_PutsBlockCenter_r(MSGC* msgc, char* str)
{
	MSG_PutsBlockCenter_h.Original(msgc, str);	
	PrintDebug("X: %X, Y: %d, W: %d, H:%d, LX: %d, LY: %d\n", msgc->x, msgc->y, msgc->width, msgc->height, msgc->lx, msgc->ly);
	AddSubtitleData(msgc, str, SubtitleAlignCenterLongest);
}

void MSG_Puts_r(MSGC* msgc, const char* str)
{
	MSG_Puts_h.Original(msgc, str);
	PrintDebug("X: %X, Y: %d, W: %d, H:%d, LX: %d, LY: %d\n", msgc->x, msgc->y, msgc->width, msgc->height, msgc->lx, msgc->ly);
	SubtitleAlignment flags;
	switch (str[0])
	{
	case 0x07:
		flags = SubtitleAlignCenterLongest;
		break;
	case 0x09:
		flags = SubtitleAlignCenter;
		break;
	default:
		flags = SubtitleAlignLeft;
		break;
	}
	AddSubtitleData(msgc, (char*)str, flags);
}

int GetTextRect(char* szText, ID3DXFont* pFont)
{
	RECT rcRect = { 0,0,0,0 };
	if (pFont)
	{
		// calculate required rect
		pFont->DrawTextA(szText, strlen(szText), &rcRect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));
	}
	// return width
	return rcRect.right - rcRect.left;
}

void DrawSubtitleTextNew(NJS_SPRITE* sp, Int n, Float pri, NJD_SPRITE attr, LATE queue_flags)
{
	if (FontLoaded)
	{
		using namespace uiscale;
		// Draw shadow
		RECT RectShadow;
		//font->DrawTextA("Come on ya big drip!", -1, &Rect2, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
		// Draw text
		RECT Rect;
		//helperFunctionsGlobal->PushScaleUI(Align::Align_Left, false, 1.0f, 1.0f);
		Rect.top = Subtitles[0].rectPos.y;
		Rect.bottom = Subtitles[0].rectPos.y+ Subtitles[0].rectSize.y;
		Rect.left = Subtitles[0].rectPos.x;
		Rect.right = Subtitles[0].rectPos.x+Subtitles[0].rectSize.x;
		//helperFunctionsGlobal->PopScaleUI();
		int drawflags = DT_TOP;
		switch (Subtitles[0].align)
		{
		case SubtitleAlignLeft:
		default:
			drawflags |= DT_LEFT;
			break;
		case SubtitleAlignCenter:
			drawflags |= DT_CENTER;
			break;
		case SubtitleAlignCenterLongest:
			drawflags |= DT_LEFT;
			int longestLineWidth = 0;
			for (int l = 0;l < Subtitles[0].numLines;l++)
			{
				longestLineWidth = max(longestLineWidth, GetTextRect(Subtitles[0].lines[l].lineText, font));
			}
			Rect.left = HorizontalResolution / 2 - longestLineWidth / 2;
			break;
		}
		// Draw text
		for (int l = 0;l < Subtitles[0].numLines;l++)
		{
			int a = cur_argb.a * 255;
			int r = 199;
			int g = 199;
			int b = 217;
			RectShadow.left = Rect.left + 2 * FontScale;
			RectShadow.top = Rect.top + 2 * FontScale;
			RectShadow.right = Rect.right + 2 * FontScale;
			RectShadow.bottom = Rect.bottom + 2 * FontScale;
			// Draw character shadows
			font->DrawTextA(Subtitles[0].lines[l].lineText, -1, &RectShadow, drawflags, ((D3DCOLOR)((((a) & 0xff) << 24) | (((0) & 0xff) << 16) | (((0) & 0xff) << 8) | ((0) & 0xff))));
			// Draw characters
			font->DrawTextA(Subtitles[0].lines[l].lineText, -1, &Rect, drawflags, ((D3DCOLOR)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff))));
			Rect.top += 28 * FontScale;
		}
	}
}

void DrawRecapNew(NJS_TEXTURE_VTX* points, Int count, Uint32 gbix, Int flag)
{
	for (int s = 0;s < NumSubtitles;s++)
	{
		RECT Rect;
		Rect.top = Subtitles[s].textPos.y;
		Rect.bottom = Subtitles[s].textPos.y + Subtitles[s].rectSize.y;
		Rect.left = Subtitles[s].rectPos.x;
		Rect.right = Subtitles[s].rectPos.x + Subtitles[s].rectSize.x;
		int a = 255;
		int r = 199;
		int g = 199;
		int b = 217;
		// Draw characters
		PrintDebug("DrawText %s\n", Subtitles[s].lines[0].lineText);
		font->DrawTextA(Subtitles[s].lines[0].lineText, Subtitles[s].lines[0].lineNumChars, &Rect, DT_CENTER, ((D3DCOLOR)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff))));
	}
	NumSubtitles = 0;
}

void DrawMissionTutorialText(NJS_SPRITE* sp, Int n, Float pri, NJD_SPRITE attr, LATE queue_flags)
{
	for (int s = 0;s < NumSubtitles;s++)
	{
		RECT Rect;
		Rect.top = Subtitles[s].textPos.y;
		Rect.bottom = Subtitles[s].textPos.y + Subtitles[s].rectSize.y;
		Rect.left = Subtitles[s].rectPos.x;
		Rect.right = Subtitles[s].rectPos.x + Subtitles[s].rectSize.x;
		int a = 255;
		int r = 199;
		int g = 199;
		int b = 217;
		// Draw characters
		PrintDebug("DrawText %s\n", Subtitles[s].lines[0].lineText);
		font->DrawTextA(Subtitles[s].lines[0].lineText, Subtitles[s].lines[0].lineNumChars, &Rect, DT_CENTER, ((D3DCOLOR)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff))));
	}
	NumSubtitles = 0;
}

void LoadFont()
{
	// Calculate font scale
	FontScale = VerticalResolution / 480.0f;
	c_hFont = CreateFontA(31 * FontScale, 0, 0, 0, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, PROOF_QUALITY, 0, "Impress BT");
	HRESULT hr = D3DXCreateFont(Direct3D_Device, c_hFont, &font);
	if (!SUCCEEDED(hr))
	{
		PrintDebug("\nUnable to create font\n");
		return;
	}
	else
		PrintDebug("\nFont loaded\n");
	FontLoaded = true;
	WriteCall((void*)0x0040D7DA, DrawSubtitleTextNew);
	//MSG_PutsCenter_h.Hook(MSG_PutsCenter_r);
	//MSG_PutsBlockCenter_h.Hook(MSG_PutsBlockCenter_r);
	MSG_Puts_h.Hook(MSG_Puts_r);
	MSG_Cls_h.Hook(MSG_Cls_r);
	WriteCall((void*)0x00642427, DrawRecapNew);
	WriteCall((void*)0x006430B2, DrawMissionTutorialText);
}
