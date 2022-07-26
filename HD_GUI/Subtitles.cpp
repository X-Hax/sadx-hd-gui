#include <SADXModLoader.h>
#include <IniFile.hpp>
#include <Trampoline.cpp>
#include "Subtitles.h"

FunctionPointer(void, DrawFileSelectWindows, (float pos_x, float pos_y, float pos_z, float width, float height), 0x4343E0);
DataPointer(float, MissionTextAlpha, 0x3C8307C);

static float RecapScreenY = 0;
static int NumberOfTextLines = 0;
static ListenToRecapMode ListenToRecap;
static float GlobalRecapAlphaForFadeout = 1.0f;
static float SubtitleShadowX = 1.0f;
static float SubtitleShadowY = 1.0f;
static float MissionScreenScale = 1.0f;
static int SubtitleFontColorR = 255;
static int SubtitleFontColorG = 255;
static int SubtitleFontColorB = 255;
static int RecapFontColorR = 255;
static int RecapFontColorG = 255;
static int RecapFontColorB = 255;
static float RecapSpacing = 5.0f;
static float SubtitleSpacing = 6.0f;

static FontOffset JapaneseCharacterData = { 60, 0, 0 };
static FontOffset JapaneseSpaceCharacterData = { 32, 0, 0 };

bool IsJapaneseCharacter(unsigned __int16 character)
{
	return character > 0x8000;
}

FontOffset const& GetFontCharacterData(unsigned __int16 character)
{
	// force space width to 32 in Japanese
	if (!TextLanguage && character == 0x20) return JapaneseSpaceCharacterData;
	else if (IsJapaneseCharacter(character)) return JapaneseCharacterData;
	else return FontCharacterData[character & 0xFF];
}

float CalculateSpacingBetweenCharacters(unsigned __int16 character_a, unsigned __int16 character_b, float halfwidth_spacing, float fullwidth_spacing)
{
	if (IsJapaneseCharacter(character_a) || IsJapaneseCharacter(character_b)) return fullwidth_spacing;
	else return halfwidth_spacing + FontCharacterData[character_b & 0xFF].offset_x;
}

float CalculateSpacingInArray(unsigned __int16* array, int next_character_index, float halfwidth_spacing, float fullwidth_spacing)
{
	if (next_character_index == 0) return 0;
	else return CalculateSpacingBetweenCharacters(array[next_character_index - 1], array[next_character_index], halfwidth_spacing, fullwidth_spacing);
}

float CalculateRecapSpacing(unsigned __int16* array, int next_character_index)
{
	return CalculateSpacingInArray(array, next_character_index, RecapSpacing, 0);
}

float CalculateSubtitleSpacing(unsigned __int16* array, int next_character_index)
{
	return CalculateSpacingInArray(array, next_character_index, SubtitleSpacing, 5.0f);
}

int GetTexidForCodepage(unsigned __int16 character)
{
	for (unsigned int i = 0; i < LengthOfArray(JapaneseSubtitleCodepage); i++)
	{
		if (character >= JapaneseSubtitleCodepage[i].StartChar && character <= JapaneseSubtitleCodepage[i].StartChar + 255) return i;
	}
	return 0;
}

void DrawCharacter(NJS_SPRITE* sprite, unsigned __int16 index, char mode, float parameter)
{
	//index = 0x9EF7;
	unsigned __int16 character = 0;
	short texid = 0;
	float FontAlpha = 1.0f;
	float pos_x_bk = sprite->p.x;
	float pos_y_bk = sprite->p.y;
	float scale_x_bk = sprite->sx;
	float scale_y_bk = sprite->sy;
	if (IsJapaneseCharacter(index))
	{
		sprite->p.x -= sprite->sx * 0.1f * 64;
		sprite->p.y -= sprite->sy * 0.1f * 64;
		sprite->sx *= 1.2f;
		sprite->sy *= 1.1f;
	}
	int v2_bk = 0;
	int uvdif = 0;
	float dif = MissionScreenScale - sprite->p.y;
	float finalscale = 1.0f;
	float depth = -1.0f;
	//PrintDebug("Mscale dif: %f\n", dif);
	if (mode == 1) // Recap screen / parameter is Y position
	{
		if (parameter >= 300) FontAlpha = max(0, (50 - (parameter - 300)) / 50.0f);
		else if (parameter <= 100) FontAlpha = max(0, (40 - (100 - parameter)) / 40.0f);
		else FontAlpha = 1.0f;
		//PrintDebug("Y: %f, FA: %f\n", YPosition, FontAlpha);
	}
	else if (mode == 2) // "File deleted" text
	{
		FontAlpha = 1.0f;
		GlobalRecapAlphaForFadeout = 1.0f;
	}
	else if (mode == 3) // Mission tutorial text / parameter is eh
	{
		FontAlpha = 1.0f - MissionTextAlpha;
		GlobalRecapAlphaForFadeout = 1.0f;
	}
	// Japanese
	if (IsJapaneseCharacter(index))
	{
		njSetTexture(&SubtitleJPTexlist);
		sprite->tlist = &SubtitleJPTexlist;
		if (index < 0x8140) character = 1;
		else character = (index - 0x8140) % 256;
		texid = GetTexidForCodepage(index);
	}
	// English etc.
	else
	{
		njSetTexture(&SubtitleTexlist);
		sprite->tlist = &SubtitleTexlist;
		character = index % 256;
		texid = 0;
	}
	//SubtitleFont[character].cy = 32;
	//SubtitleFont[character].sy = 32;
	//SubtitleFont[character].v2 = SubtitleFont[character].v1+8;
	SubtitleFont[character].texid = texid;
	// Apply transformations
	if (mode == 3)
	{
		depth = -1.15f;
		v2_bk = SubtitleFont[character].v2;
		if (dif <= 47.0f)
		{
			finalscale = max(0, dif / 47.0f);
			if (finalscale < 0.28f) finalscale = 0;
			uvdif = SubtitleFont[character].v2 - SubtitleFont[character].v1;
			SubtitleFont[character].v2 = SubtitleFont[character].v1 + int((float)uvdif * finalscale);
			SubtitleFont[character].sy = int(64.0f* finalscale);
			sprite->sy *= float(SubtitleFont[character].sy)/64.0f;
			//PrintDebug("Scale: %f, sy: %f, texsy: %d, v2:%d \n", finalscale, sprite->sy, SubtitleFont[character].sy, SubtitleFont[character].v2);
		}
	}
	// Draw font shadow
	if (SubtitleShadowX != 0 || SubtitleShadowY != 0)
	{
		if (mode) SetMaterialAndSpriteColor_Float(max(0, FontAlpha * GlobalRecapAlphaForFadeout), 0, 0, 0);
		else SetMaterialAndSpriteColor_Float(GlobalSpriteColor.a, 0, 0, 0);
		sprite->p.x += SubtitleShadowX;
		sprite->p.y += SubtitleShadowY;
		if (mode != 3 || !TextLanguage) njDrawSprite2D_DrawNow(sprite, character, depth, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
		else njDrawSprite2D_Queue(sprite, character, -1.15f, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR, (QueuedModelFlagsB)0);
		sprite->p.x -= SubtitleShadowX;
		sprite->p.y -= SubtitleShadowY;
	}
	// Draw text
	//PrintDebug("Drawing character %X, texid %d, id %d, alpha %f, y %f\n", index, texid, character, FontAlpha, parameter);
	if (mode) SetMaterialAndSpriteColor_Float(max(0, FontAlpha * GlobalRecapAlphaForFadeout), max(0, GlobalRecapAlphaForFadeout * RecapFontColorR / 255.0f), max(0, GlobalRecapAlphaForFadeout * RecapFontColorG / 255.0f), max(0, GlobalRecapAlphaForFadeout * RecapFontColorB / 255.0f));
	else SetMaterialAndSpriteColor_Float(GlobalSpriteColor.a, SubtitleFontColorR / 255.0f, SubtitleFontColorG / 255.0f, SubtitleFontColorB / 255.0f);
	//SetMaterialAndSpriteColor_Float(1, 1, 1, 1);
	// Japanese mission tutorial text should be drawn immediately to avoid overriding texture ids by next characters
	if (mode != 3 || !TextLanguage) njDrawSprite2D_DrawNow(sprite, character, depth, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
	else njDrawSprite2D_Queue(sprite, character, -1.15f, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR, (QueuedModelFlagsB)0);
	// Remove transformations
	if (mode == 3)
	{
		SubtitleFont[character].sy = 64;
		SubtitleFont[character].v2 = v2_bk;
	}
	sprite->p.x = pos_x_bk;
	sprite->p.y = pos_y_bk;
	sprite->sx = scale_x_bk;
	sprite->sy = scale_y_bk;
}

void AddSubtitleCharacter(unsigned __int16* array, unsigned __int16 character)
{
	for (int i = 0; i < 255; i++)
	{
		if (array[i] == 0)
		{
			array[i] = character;
			array++;
			return;
		}
	}
}

void ClearTextArray(unsigned __int16* array)
{
	for (int i = 0; i < 255; i++)
	{
		array[i] = 0;
	}
}

void ParseSubtitle(const char* string)
{
	unsigned __int16* array = nullptr;
	unsigned __int16 character = 0;

	if (ListenToRecap == ListenToRecapMode::Listen)
	{
		NumberOfTextLines++;
	}

	if (GameMode != GameModes_Adventure_Story && GameMode != 0x14) //Check if not on recap screen or mission tutorial screen
	{
		array = SubtitleArray;
	}
	else array = RecapArray[NumberOfTextLines - 1].LineString_S;

	ClearTextArray(array);
	/*
	PrintDebug("Text string:");
	for (int i = 0; i < strlen(string); i++)
	{
		PrintDebug("%X ", string[i]);
	}
	PrintDebug("Text string end\n");
	*/
	for (unsigned int i = 0; i < strlen(string); i++)
	{
		// Here's a bunch of hardcoded Japanese characters because the game uses it in English too and because I'm tired of this shit
		if (TextLanguage == 1 && string[i] == 0xFFFFFF81)
		{
			// Square
			if (string[i + 1] == 0xFFFFFFA1)
			{
				AddSubtitleCharacter(array, 0x01);
			}
			// ...
			if (string[i + 1] == 0x63)
			{
				AddSubtitleCharacter(array, 0x2);
			}
			// '
			if (string[i + 1] == 0x65)
			{
				AddSubtitleCharacter(array, 0x27);
			}
			// !
			if (string[i + 1] == 0x49)
			{
				AddSubtitleCharacter(array, 0x21);
			}
			// ?
			if (string[i + 1] == 0x48)
			{
				AddSubtitleCharacter(array, 0x3F);
			}
			// Skip the next character because it's zenkaku
			AddSubtitleCharacter(array, 0x20);
			i++;
		}
		// Read as zenkaku
		else if (TextLanguage <= 1 && string[i] & 0xFFFFFF00)
		{
			character = 0x100 * (string[i] & 0xFF) + abs(string[i + 1] & 0xFF);
			//PrintDebug("%X ", character);
			i++;
			AddSubtitleCharacter(array, character);
		}
		// Read as hankaku
		else
		{
			character = string[i] & 0xFF;
			//PrintDebug("%X ", character);
			AddSubtitleCharacter(array, character);
		}
	}
	// Calculate recap line length
	if (ListenToRecap == ListenToRecapMode::Listen)
	{
		TextLine& LastRecapArray = RecapArray[NumberOfTextLines - 1];
		for (int i = 0; i < 255; i++)
		{
			// Calculate the line's width in pixels (at 1.0x) with character spacing
			if (array[i] == 0) break;
			else if (array[i] != 0x07 && array[i] != 0x09)
			{
				LastRecapArray.LineLength += (int)(CalculateRecapSpacing(LastRecapArray.LineString_S, i) + GetFontCharacterData(LastRecapArray.LineString_S[i]).width);
			}
		}
	}
}

static void DisplaySubtitleThing_r(SubtitleThing* a1, const char* a2);
static Trampoline DisplaySubtitleThing_t(0x40E570, 0x40E575, DisplaySubtitleThing_r);
static void __cdecl DisplaySubtitleThing_r(SubtitleThing* a1, const char* a2)
{
	auto original = reinterpret_cast<decltype(DisplaySubtitleThing_r)*>(DisplaySubtitleThing_t.Target());
	original(a1, a2);
	//PrintDebug("Subtitle: %s\n", a2);
	ParseSubtitle(a2);
}

static void DisplayRecapThing_r(ObjectMaster* a1);
static Trampoline DisplayRecapThing_t(0x642300, 0x642306, DisplayRecapThing_r);
static void __cdecl DisplayRecapThing_r(ObjectMaster* a1)
{
	EntityData1* SubtitleEntity = a1->Data1;
	auto original = reinterpret_cast<decltype(DisplayRecapThing_r)*>(DisplayRecapThing_t.Target());
	original(a1);
	RecapScreenY = SubtitleEntity->Position.y;
	//PrintDebug("Ass: %f and %f", SubtitleEntity->Position.x, SubtitleEntity->Position.y);
}

void LoadPVMHook_Tutorial()
{
	LoadPVM("SUBTITLE", &SubtitleTexlist);
	LoadPVM("SUBTITLE_JP", &SubtitleJPTexlist);
}

void LoadFontdata(const IniFile* config)
{
	//PrintDebug("\n");
	SubtitleFontColorR = config->getInt("General", "SubtitleFontColorR", 255);
	SubtitleFontColorG = config->getInt("General", "SubtitleFontColorG", 255);
	SubtitleFontColorB = config->getInt("General", "SubtitleFontColorB", 255);
	SubtitleSpacing = config->getFloat("General", "SubtitleSpacing", 6.0f);
	RecapFontColorR = config->getInt("General", "RecapFontColorR", 255);
	RecapFontColorG = config->getInt("General", "RecapFontColorG", 255);
	RecapFontColorB = config->getInt("General", "RecapFontColorB", 255);
	RecapSpacing = config->getFloat("General", "RecapSpacing", 6.0f);
	SubtitleShadowX = config->getFloat("General", "ShadowXOffset", 1.0f);
	SubtitleShadowY = config->getFloat("General", "ShadowYOffset", 1.0f);
	for (unsigned int i = 0; i < LengthOfArray(FontCharacterData); i++)
	{
		FontCharacterData[i].width = 32;
		FontCharacterData[i].offset_x = 0;
		FontCharacterData[i].offset_y = 0;
		//PrintDebug("[%d]\n", i);
		std::string w = std::to_string(i);
		FontCharacterData[i].width = config->getInt(w, "Width");
		FontCharacterData[i].offset_x = config->getInt(w, "OffsetX");
		FontCharacterData[i].offset_y = config->getInt(w, "OffsetY");
		//PrintDebug("Width=%d\n", FontCharacterData[i].width);
		//PrintDebug("OffsetX=%d\n", FontCharacterData[i].offset_x);
		//PrintDebug("OffsetY=%d\n", FontCharacterData[i].offset_y);
		//PrintDebug("\n");
	}
}

void BuildFontINI()
{
	PrintDebug("\n");
	PrintDebug("[General]\n");
	PrintDebug("SubtitleFontColorR=199\n");
	PrintDebug("SubtitleFontColorG=199\n");
	PrintDebug("SubtitleFontColorB=217\n");
	PrintDebug("SubtitleSpacing=6.0\n");
	PrintDebug("RecapFontColorR=199\n");
	PrintDebug("RecapFontColorG=199\n");
	PrintDebug("RecapFontColorB=217\n");
	PrintDebug("RecapFontColorB=217\n");
	PrintDebug("RecapSpacing=5.0\n");
	PrintDebug("\n");
	for (unsigned int i = 0; i < LengthOfArray(FontCharacterData); i++)
	{
		PrintDebug("[%d]\n", i);
		PrintDebug("Width=%d\n", FontCharacterData[i].width);
		PrintDebug("OffsetX=%d\n", FontCharacterData[i].offset_x);
		PrintDebug("OffsetY=%d\n", FontCharacterData[i].offset_y);
		PrintDebug("\n");
	}
}

int CalculateLineLength(int start_char)
{
	int result = 0;
	for (unsigned int i = start_char; i < LengthOfArray(SubtitleArray); i++)
	{
		if (SubtitleArray[i] == 0 || SubtitleArray[i] == 0x0A)
		{
			break;
		}
		else if (SubtitleArray[i] != 0x09 && SubtitleArray[i] != 0x07)
		{
			result += (int)(CalculateSubtitleSpacing(SubtitleArray, i) + GetFontCharacterData(SubtitleArray[i]).width);
		}
	}
	//PrintDebug("Line length for line starting at %d: %d\n", start_char, result);
	return result;
}

float CalculateLongestLineLength()
{
	/*PrintDebug("\nSubtitle array:");
	for (int i = 0; i < LengthOfArray(SubtitleArray); i++)
	{
		PrintDebug("%X ", SubtitleArray[i] % 256);
	}
	PrintDebug("\nSubtitle array over\n");*/
	int linenumber = 0;
	int line1 = 0;
	int line2 = 0;
	int line3 = 0;
	int line4 = 0;
	int result = 0;
	int spaceadd = 0;
	for (unsigned int cursor = 0; cursor < LengthOfArray(SubtitleArray); cursor++)
	{
		if (SubtitleArray[cursor] == 0x0A)
		{
			linenumber++;
		}
		else if (SubtitleArray[cursor] == 0)
		{
			result = max(max(line1, line2), max(line3, line4));
			goto results;
			break;
		}
		else if (SubtitleArray[cursor] != 0x09 && SubtitleArray[cursor] != 0x07)
		{
			spaceadd = (int)(CalculateSubtitleSpacing(SubtitleArray, cursor) + GetFontCharacterData(SubtitleArray[cursor]).width);
			//Add spacing
			switch (linenumber)
			{
			case 0:
				line1 += spaceadd;
				break;
			case 1:
				line2 += spaceadd;
				break;
			case 2:
				line3 += spaceadd;
				break;
			case 3:
				line4 += spaceadd;
				break;
			}
		}
	}
	goto results;
results:
	//PrintDebug("1: %d, 2: %d, 3: %d, 4: %d, longest line: %d\n", line1, line2, line3, line4, result);
	return (float)result;
}

void DrawMissionTutorialTextHook(NJS_SPRITE* sp, Int n, Float pri, NJD_SPRITE attr, QueuedModelFlagsB queue_flags)
{
	float ResolutionScale = (float)VerticalResolution / 480.0f;
	float FontAlpha = 1.0f;
	float OldOffsetX = 0;
	float OffsetY = 0;
	NJS_SPRITE SubtitleCharacterSprite = {{ 0, 0, 0 }, 0.4f, 0.4f, 0, &SubtitleTexlist, SubtitleFont};
	//Set up scaling
	SubtitleCharacterSprite.sx = 0.4f;
	SubtitleCharacterSprite.sy = 0.4f;
	for (int u = 0; u < NumberOfTextLines; u++)
	{
		//PrintDebug("Length for line %d: %i\n", u, RecapArray[u].LineLength);
		// Calculate centering
		OldOffsetX = sp->p.x;
		SubtitleCharacterSprite.p.y = sp->p.y + OffsetY;
		for (int i = 0; i < 255; i++)
		{
			if (RecapArray[u].LineString_S[i] == 0) break;
			// Calculate X position
			if (i == 0) SubtitleCharacterSprite.p.x = OldOffsetX;
			else
			{
				SubtitleCharacterSprite.p.x = OldOffsetX + SubtitleCharacterSprite.sx * (GetFontCharacterData(RecapArray[u].LineString_S[i - 1]).width + CalculateRecapSpacing(RecapArray[u].LineString_S, i));
			}
			// Draw
			if (RecapArray[u].LineString_S[i] != 0x0A && RecapArray[u].LineString_S[i] != 0x07 && RecapArray[u].LineString_S[i] != 0x09)
			{
				DrawCharacter(&SubtitleCharacterSprite, RecapArray[u].LineString_S[i], 3, OffsetY);
			}
			OldOffsetX = SubtitleCharacterSprite.p.x;
		}
		OffsetY += 68.0f * SubtitleCharacterSprite.sx;
	}
}

void DrawSubtitleHook(NJS_SPRITE* sp, Int n, Float pri, NJD_SPRITE attr, QueuedModelFlagsB queue_flags)
{
	NJS_SPRITE SubtitleCharacterSprite = {{ 0, 0, 0 }, 0.4f, 0.4f, 0, &SubtitleTexlist, SubtitleFont};
	float scaled320 = 320.0f;
	if (sp->p.x <= 90) scaled320 = 320.0f; else scaled320 = HorizontalResolution / 2.0f;
	// Calculate newlines
	float HorizontalOffset_Initial = 0;
	float HorizontalOffset_Current = 0;
	float VerticalOffset = 0;
	int LineStart = 0;
	// Set subtitle position and texlist
	SubtitleCharacterSprite.p.x = sp->p.x;
	SubtitleCharacterSprite.p.y = sp->p.y;
	// Set font scale - for Mission Stat screen it needs to be smaller to fit mission descriptions into the small frame
	if (MissionScreenState)
	{
		SubtitleCharacterSprite.sx = 0.28f;
		SubtitleCharacterSprite.sy = 0.28f;
	}
	else
	{
		SubtitleCharacterSprite.sx = 0.4f;
		SubtitleCharacterSprite.sy = 0.4f;
	}
	for (unsigned int i = 0; i < LengthOfArray(SubtitleArray); i++)
	{
		if (SubtitleArray[i] == 0) break;
		// Calculate horizontal centering
		if (MissionScreenState) HorizontalOffset_Initial = 7.0f * 32.0f * SubtitleCharacterSprite.sx;
		// Fixed offset for mission stat screen
		else
		{
			// 0x09 as the first character centers the line
			if (SubtitleArray[LineStart] == 0x09)
			{
				HorizontalOffset_Initial = scaled320 - (CalculateLineLength(LineStart) / 2.0f) * SubtitleCharacterSprite.sx;
			}
			// Otherwise (e.g. two lines) center by the longest line
			else
			{
				HorizontalOffset_Initial = scaled320 - (CalculateLongestLineLength() / 2.0f) * SubtitleCharacterSprite.sx;
			}
		}
		// If not newline or spacing, draw the character
		if (SubtitleArray[i] != 0x0A && SubtitleArray[i] != 0x09 && SubtitleArray[i] != 0x07 && SubtitleArray[i] != 0i8)
		{
			// Add spacing
			if (i != 0)
				HorizontalOffset_Current += SubtitleCharacterSprite.sx * (GetFontCharacterData(SubtitleArray[i - 1]).width + CalculateSubtitleSpacing(SubtitleArray, i));
			//PrintDebug("Letter %d, width %d, hz:%f\n", SubtitleArray[i] & 0xFF, FontCharacterData[SubtitleArray[i] & 0xFF].width, HorizontalOffset_Current);
			SubtitleCharacterSprite.p.x = HorizontalOffset_Initial + HorizontalOffset_Current;
			SubtitleCharacterSprite.p.y = sp->p.y + VerticalOffset + SubtitleCharacterSprite.sx * GetFontCharacterData(SubtitleArray[i]).offset_y;
			DrawCharacter(&SubtitleCharacterSprite, SubtitleArray[i], 0, 0);
		}
		else if (SubtitleArray[i] == 0x0A)
		{
			VerticalOffset += 68.0f * SubtitleCharacterSprite.sx;
			HorizontalOffset_Current = 0;
			LineStart = i + 1;
		}
	}
}

void DrawSaveDeletedTextHook(NJS_TEXTURE_VTX* points, Int count, Uint32 gbix, Int flag)
{
	//PrintDebug("XP: %f\n", points->x);
	float scaled320 = points->x <= 60 ? 320.0f: HorizontalResolution / 2.0f;
	float ResolutionScale = (float)VerticalResolution / 480.0f;
	float FontAlpha = 1.0f;
	float OldOffsetX = points[0].x;
	NJS_SPRITE SubtitleCharacterSprite = {{ 0, 0, 0 }, 0.4f, 0.4f, 0, &SubtitleTexlist, SubtitleFont};
	OldOffsetX = scaled320 - (CalculateLineLength(0) / 2.0f) * SubtitleCharacterSprite.sx;
	// Set up scaling
	SubtitleCharacterSprite.sx = 0.4f;
	SubtitleCharacterSprite.sy = 0.4f;
	for (unsigned int i = 0; i < LengthOfArray(SubtitleArray); i++)
	{
		if (SubtitleArray[i] == 0) 
			break;
		// Calculate X position
		if (i == 0) 
			SubtitleCharacterSprite.p.x = OldOffsetX;
		else if (SubtitleArray[i] != 0x09)
		{
			SubtitleCharacterSprite.p.x = OldOffsetX + SubtitleCharacterSprite.sx * (GetFontCharacterData(SubtitleArray[i - 1]).width + CalculateSubtitleSpacing(SubtitleArray, i));
		}
		// Set Y position
		SubtitleCharacterSprite.p.y = points[0].y;
		// Draw character
		DrawCharacter(&SubtitleCharacterSprite, SubtitleArray[i], 2, 0);
		OldOffsetX = SubtitleCharacterSprite.p.x;
	}
}

void DrawRecapTextHook(NJS_TEXTURE_VTX* points, Int count, Uint32 gbix, Int flag)
{
	float ResolutionScale = (float)VerticalResolution / 480.0f;
	float FontAlpha = 1.0f;
	float OldOffsetX = 0;
	float OffsetY = 0;
	NJS_SPRITE SubtitleCharacterSprite = {{ 0, 0, 0 }, 0.4f, 0.4f, 0, &SubtitleTexlist, SubtitleFont};
	// Set up scaling
	SubtitleCharacterSprite.sx = 0.4f * ResolutionScale;
	SubtitleCharacterSprite.sy = 0.4f * ResolutionScale;
	float LeftCorner = ((float)HorizontalResolution - ResolutionScale * 640.0f)/2.0f;
	//PrintDebug("Left: %f\n", LeftCorner);
	for (int u = 0; u < NumberOfTextLines; u++)
	{
		//PrintDebug("Length for line %d: %i\n", u, RecapArray[u].LineLength);
		// Calculate centering
		if (RecapArray[u].LineString_S[0] == 0x07 || RecapArray[u].LineString_S[0] == 0x09) OldOffsetX = ((float)HorizontalResolution / 2.0f) - SubtitleCharacterSprite.sx * (RecapArray[u].LineLength / 2);
		else OldOffsetX = LeftCorner + 64 * ResolutionScale;
		for (int i = 0; i < 255; i++)
		{
			if (RecapArray[u].LineString_S[i] == 0) break;
			// Calculate X position
			if (i == 0) SubtitleCharacterSprite.p.x = OldOffsetX;
			else
			{
				SubtitleCharacterSprite.p.x = OldOffsetX + SubtitleCharacterSprite.sx * (GetFontCharacterData(RecapArray[u].LineString_S[i - 1]).width + CalculateRecapSpacing(RecapArray[u].LineString_S, i));
			}
			// Calculate Y position
			OffsetY = SubtitleCharacterSprite.sx * GetFontCharacterData(RecapArray[u].LineString_S[i]).offset_y;
			SubtitleCharacterSprite.p.y = (u * 40 + 118 + RecapScreenY) * ResolutionScale + OffsetY;
			// Draw
			DrawCharacter(&SubtitleCharacterSprite, RecapArray[u].LineString_S[i], 1, (SubtitleCharacterSprite.p.y - OffsetY) / ResolutionScale);
			OldOffsetX = SubtitleCharacterSprite.p.x;
		}
	}
}

void RecapStart(SubtitleThing* a1)
{
	//PrintDebug("Recap start\n");
	if (ListenToRecap != ListenToRecapMode::Done)
	{
		ListenToRecap = ListenToRecapMode::Listen;
		NumberOfTextLines = 0;
		for (unsigned int i = 0; i < LengthOfArray(RecapArray); i++)
		{
			RecapArray[i].LineLength = 0;
			ClearTextArray(RecapArray[i].LineString_S);
		}
	}
	DoTextThing_Start(a1);
}

void MissionStart(SubtitleThing* a1)
{
	//PrintDebug("Mission start\n");
	if (ListenToRecap != ListenToRecapMode::Done)
	{
		ListenToRecap = ListenToRecapMode::Listen;
		NumberOfTextLines = 0;
		for (unsigned int i = 0; i < LengthOfArray(RecapArray); i++)
		{
			RecapArray[i].LineLength = 0;
			ClearTextArray(RecapArray[i].LineString_S);
		}
	}
	DoTextThing_Start(a1);
}

void RecapStop(SubtitleThing* a1)
{
	//PrintDebug("Recap stop\n");
	if (ListenToRecap != ListenToRecapMode::Done)
	{
		ListenToRecap = ListenToRecapMode::Done;
	}
	DoTextThing_Stop(a1);
}

void LoadPVMHook_Recap(const char* filename, NJS_TEXLIST* texlist)
{
	LoadPVM(filename, texlist);
	LoadPVM("SUBTITLE", &SubtitleTexlist);
	LoadPVM("SUBTITLE_JP", &SubtitleJPTexlist);
}

static void SkyChaseInit_r();
static Trampoline SkyChaseInit_t(0x6281A0, 0x6281A7, SkyChaseInit_r);
static void __cdecl SkyChaseInit_r()
{
	auto original = reinterpret_cast<decltype(SkyChaseInit_r)*>(SkyChaseInit_t.Target());
	original();
	LoadPVM("SUBTITLE", &SubtitleTexlist);
	LoadPVM("SUBTITLE_JP", &SubtitleJPTexlist);
}

static void UnlockEmblemCountShitr(ObjectMaster* a1);
static Trampoline UnlockEmblemCountShitt(0x4B55B0, 0x4B55B6, UnlockEmblemCountShitr);
static void __cdecl UnlockEmblemCountShitr(ObjectMaster* a1)
{
	auto original = reinterpret_cast<decltype(UnlockEmblemCountShitr)*>(UnlockEmblemCountShitt.Target());
	original(a1);
	LoadPVM("SUBTITLE", &SubtitleTexlist);
	LoadPVM("SUBTITLE_JP", &SubtitleJPTexlist);
}

void MissionStop()
{
	if (ListenToRecap != ListenToRecapMode::Done)
	{
		ListenToRecap = ListenToRecapMode::Done;
	}
}

void MissionWindowHook(float pos_x, float pos_y, float pos_z, float width, float height)
{
	DrawFileSelectWindows(pos_x, pos_y, pos_z, width, height);
	//PrintDebug("Height: %f\n", height);
	MissionScreenScale = pos_y+height;
}

void Subtitles_Init(const char* path, const HelperFunctions& helperFunctions)
{
	// Subtitle hooks
	WriteCall((void*)0x6431D3, MissionWindowHook);
	// Disable all the stuff that sets up the "Now saving" text but nothing else
	WriteData<5>((char*)0x40BE6C, 0x90);
	WriteData<5>((char*)0x40BE92, 0x90);
	WriteData<5>((char*)0x40BEA1, 0x90);
	WriteData<5>((char*)0x40BEAB, 0x90);
	WriteData<5>((char*)0x40BF27, 0x90);
	WriteCall((void*)0x40D7DA, DrawSubtitleHook);
	WriteCall((void*)0x643DBA, LoadPVMHook_Recap);
	WriteCall((void*)0x642363, RecapStart);
	WriteCall((void*)0x6423EE, RecapStop);
	WriteCall((void*)0x642427, DrawRecapTextHook);
	WriteCall((void*)0x421906, DrawSaveDeletedTextHook);
	WriteCall((void*)0x642EF6, MissionStart);
	WriteCall((void*)0x642F36, MissionStop);
	WriteCall((void*)0x6430B2, DrawMissionTutorialTextHook);
	WriteCall((void*)0x6421E7, LoadPVMHook_Tutorial);
	// Add subtitle texlist to common object textures
	helperFunctions.RegisterCommonObjectPVM(SubtitlePVMEntry);
	helperFunctions.RegisterCommonObjectPVM(SubtitleJPPVMEntry);
	// Load fontdata settings
	const std::string s_path(path);
	const std::string s_config_ini(helperFunctions.GetReplaceablePath("SYSTEM\\fontdata.ini"));
	const IniFile* const config = new IniFile(s_config_ini);
	LoadFontdata(config);
	delete config;
}

void Subtitles_OnFrame()
{
	if (GameMode != GameModes_Adventure_Story)
	{
		ListenToRecap = ListenToRecapMode::Idle;
		GlobalRecapAlphaForFadeout = 1.0f;
	}
	else if (RecapScreenMode == 2 && GlobalRecapAlphaForFadeout > 0.0f) 
		GlobalRecapAlphaForFadeout = max(0.0f, GlobalRecapAlphaForFadeout - 0.05f);
}