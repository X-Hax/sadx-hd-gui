#include <SADXModLoader.h>
#include "CommonObjects.h"
#include "Objects_Shooting.h"
#include <stdlib.h>
#include <stdio.h>
#include "TutorialStuff.h"
#include "SubtitleData.h"
#include <Trampoline.h>
#include <IniFile.hpp>

#define ReplacePVMX(a) helperFunctions.ReplaceFile("system\\" a ".PVM", "system\\" a "_HD.PVM")

#define ReplacePNG_GoalRing(a) do { \
	_snprintf_s(pathbuf, LengthOfArray(pathbuf), "%s\\textures\\pvr_mission_goalring\\index.txt", path); \
	helperFunctions.ReplaceFile("system\\" a ".PVR", pathbuf); \
} while (0)

#define ReplacePNG_Common(a) do { \
	_snprintf_s(pathbuf, LengthOfArray(pathbuf), "%s\\textures\\pvr_common\\index.txt", path); \
	helperFunctions.ReplaceFile("system\\" a ".PVR", pathbuf); \
} while (0)

#define ReplacePNG_MissionE(a) do { \
	_snprintf_s(pathbuf, LengthOfArray(pathbuf), "%s\\textures\\pvr_mission_en\\index.txt", path); \
	helperFunctions.ReplaceFile("system\\" a ".PVR", pathbuf); \
} while (0)

#define ReplacePNG_MissionJ(a) do { \
	_snprintf_s(pathbuf, LengthOfArray(pathbuf), "%s\\textures\\pvr_mission_jp\\index.txt", path); \
	helperFunctions.ReplaceFile("system\\" a ".PVR", pathbuf); \
} while (0)

#define ReplacePNG_StageE(a) do { \
	_snprintf_s(pathbuf, LengthOfArray(pathbuf), "%s\\textures\\pvr_stage_en\\index.txt", path); \
	helperFunctions.ReplaceFile("system\\" a ".PVR", pathbuf); \
} while (0)

#define ReplacePNG_StageJ(a) do { \
	_snprintf_s(pathbuf, LengthOfArray(pathbuf), "%s\\textures\\pvr_stage_jp\\index.txt", path); \
	helperFunctions.ReplaceFile("system\\" a ".PVR", pathbuf); \
} while (0)

static float Options_ArrowScale = 0.0f;
static float Options_ArrowScaleAmount = 0.1f;

static int PSInt = 0;
static float PSsX = 0;
static float PSsY = 0;
static float PSsZ = 0;
static int BSInt = 0;
static float BSsX = 0;
static float BSsY = 0;
static float BSsZ = 0;
static float f480_Fixed = 0;
static float f640_Fixed = 0;

float PadManuXOffset_F = 175.0f;
float PadManuXOffset_General = 220.0f;
float PadManuXOffset_J = 200.0f;
float PadManuYOffset = 136.0f;
float PadManuYOffset2 = 105.0f;
float PadManuYMultiplier = 1.0f;

//Subtitle/recap stuff
static char SubtitleString[1024];
static int SubtitleCharacterCount = 0;
static float RecapScreenY = 0;
static int NumberOfRecapLines = 0;
static int ListenToRecap = 0; //0 - idle, 1 - listen, 2 - done listening
static float GlobalRecapAlphaForFadeout = 0.0f;

static int SubtitleFontColorR = 255;
static int SubtitleFontColorG = 255;
static int SubtitleFontColorB = 255;
static int RecapFontColorR = 255;
static int RecapFontColorG = 255;
static int RecapFontColorB = 255;
static float RecapSpacing = 5.0f;
static float SubtitleSpacing = 6.0f;

NJS_TEXANIM RandomRingIconPart_TEXANIM = { 32, 32, 16, 16, 0, 0, 255, 255, 95, 0x20 };

void FileIcon_Hook(int that_cant_be_right, float Texture_X, float Texture_Y, float Texture_Z)
{
	float Arrow1_X;
	float Arrow1_Y;
	float Arrow2_X;
	float Arrow2_Y;
	float Arrow1Center_X = 251.0f;
	float Arrow1Center_Y = 268.0f;
	float Arrow2Center_X = 218.0f;
	float Arrow2Center_Y = 322.0f;
	float Arrow1Scale = 0.0f + Options_ArrowScale;
	float Arrow2Scale = 0.5f - Options_ArrowScale;
	Arrow1_X = (Arrow1Center_X - 64 * Arrow1Scale) - 320.0f + HorizontalStretch * 320.0f;
	Arrow1_Y = (Arrow1Center_Y - 64 * Arrow1Scale) - 240.0f + VerticalStretch * 240.0f;
	Arrow2_X = (Arrow2Center_X - 64 * Arrow2Scale) - 320.0f + HorizontalStretch * 320.0f;
	Arrow2_Y = (Arrow2Center_Y - 64 * Arrow2Scale) - 240.0f + VerticalStretch * 240.0f;
	njTextureShadingMode(1);
	DisplayScreenTexture(44, Texture_X, Texture_Y, Texture_Z);
	DrawBG(45, Arrow1_X, Arrow1_Y, Texture_Z, Arrow1Scale, Arrow1Scale);
	DrawBG(46, Arrow2_X, Arrow2_Y, Texture_Z, Arrow2Scale, Arrow2Scale);
	njTextureShadingMode(2);
}

void DrawShadow_Hook(int texnum, float x, float y, float z, float scaleX, float scaleY)
{
	njTextureShadingMode(1);
	DrawBG(texnum, x, y, z, scaleX, scaleY);
	njTextureShadingMode(2);
}

void DrawTexture_Hook(int that_cant_be_right, float x, float y, float z)
{
	njTextureShadingMode(1);
	DisplayScreenTexture(that_cant_be_right, x, y, z);
	njTextureShadingMode(2);
}

void DisplayScreenTexture_AlwaysTop(int that_cant_be_right, float x, float y, float z)
{
	Direct3D_SetZFunc(7u);
	Direct3D_EnableZWrite(0);
	DisplayScreenTexture(that_cant_be_right, x, y, z);
	Direct3D_EnableZWrite(1);
	Direct3D_SetZFunc(3u);
}

void ScreenFadeFix(float left, float top, float right, float bottom, float depth, int color, QueuedModelFlagsB queueflags)
{
	DrawRect_Queue(-50.0f, -50.0f, HorizontalResolution + 50.0f, VerticalResolution + 50.0f, 32048.0f, color, QueuedModelFlagsB_EnableZWrite);
}

void RenderShittyTextures(int texnum, float x, float y, float z, float scaleX, float scaleY)
{
	DrawBG(texnum, x, y, z, scaleX, scaleY);
	DoColorGradientThingMaybe(0xFF0016FF, 0xFF002EFF, 0xFF0016FF, 0xFF002EFF);
	DisplayScreenTexture(BSInt, BSsX, BSsY, BSsZ);
	DoColorGradientThingMaybe(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu);
	DisplayScreenTexture(PSInt, PSsX, PSsY, PSsZ);
}

void RetrieveBottomThingStuff(int texnum, float x, float y, float z)
{
	BSInt = texnum;
	BSsX = x;
	BSsY = y;
	BSsZ = z;
}

void RetrievePlayerSelectStuff(int that_cant_be_right, float x, float y, float z)
{
	PSInt = that_cant_be_right;
	PSsX = x;
	PSsY = y;
	PSsZ = z;
}

void ScaleCharselJapaneseText_LikeSeriouslyWTF(int that_cant_be_right, float x, float y, float z)
{
	DrawBG(that_cant_be_right, x+5.0f, y, z, 0.77999997f, 0.77999997f);
}

void HelpAvaSquareThing(unsigned __int8 n, float x, float y, float z, float scaleX, float scaleY)
{
	Direct3D_EnableZWrite(0);
	DrawBG_ava_square_a(n, x, y, z, scaleX, scaleY);
	Direct3D_EnableZWrite(1);
}

void DrawSprite_Hook(NJS_SPRITE *sp, Int n, Float pri, NJD_SPRITE attr, QueuedModelFlagsB queue_flags)
{
	njTextureShadingMode(1);
	njDrawSprite2D_Queue(sp, n, pri, attr, queue_flags);
	njTextureShadingMode(2);
}

void GreenRect_Wrapper(float x, float y, float z, float width, float height)
{
	njTextureShadingMode(1);
	GreenMenuRect_Draw(x, y, z, width, height);
	njTextureShadingMode(2);
}

//Subtitle stuff

void ParseSubtitle(const char* string)
{
	if (GameMode != GameModes_Adventure_Story) //Check if not on recap screen
	{
		strcpy_s(SubtitleString, string);
		SubtitleCharacterCount = strlen(string);
		for (int i = 0; i < strlen(string); i++)
		{
			//PrintDebug("%X ", string[i] & 0xFF);
			if (string[i] == 0xFFFFFF81)
			{
				//Square
				if (string[i + 1] == 0xFFFFFFA1)
				{
					SubtitleString[i] = 0x01;
					SubtitleString[i + 1] = 0x20;
				}
				//...
				if (string[i + 1] == 0x63)
				{
					SubtitleString[i] = 0x00;
					SubtitleString[i + 1] = 0x20;
				}
				//!
				if (string[i + 1] == 0x49)
				{
					SubtitleString[i] = 0x33;
					SubtitleString[i + 1] = 0x20;
				}
				//?
				if (string[i + 1] == 0x48)
				{
					SubtitleString[i] = 0x63;
					SubtitleString[i + 1] = 0x20;
				}
			}
			else
			{
				if (string[max(0, i - 1)] != 0xFFFFFF81) SubtitleString[i] = string[i] & 0xFF;
			}
		}
	}
	else
	{
		if (ListenToRecap == 1)
		{
			strcpy_s(SubtitleString, string);
			for (int i = 0; i < strlen(string); i++)
			{
				if (string[i] == 0xFFFFFF81)
				{
					//Square
					if (string[i + 1] == 0xFFFFFFA1)
					{
						SubtitleString[i] = 0x01;
						SubtitleString[i + 1] = 0x20;
					}
					//...
					if (string[i + 1] == 0x63)
					{
						SubtitleString[i] = 0x00;
						SubtitleString[i + 1] = 0x20;
					}
					//!
					if (string[i + 1] == 0x49)
					{
						SubtitleString[i] = 0x33;
						SubtitleString[i + 1] = 0x20;
					}
					//?
					if (string[i + 1] == 0x48)
					{
						SubtitleString[i] = 0x63;
						SubtitleString[i + 1] = 0x20;
					}
				}
				else
				{
					if (string[max(0, i - 1)] != 0xFFFFFF81) SubtitleString[i] = string[i] & 0xFF;
				}
			}
			//Add line to recap array
			NumberOfRecapLines++;
			strcpy_s(RecapLineArray[NumberOfRecapLines - 1].LineString, SubtitleString);
			for (int i = 0; i < strlen(SubtitleString); i++)
			{
				//Calculate the line's width in pixels (at 1.0x) with character spacing
				if (SubtitleString[i] != 0x07 && SubtitleString[i] != 0x09)	RecapLineArray[NumberOfRecapLines - 1].LineLength = RecapLineArray[NumberOfRecapLines - 1].LineLength + FontCharacterData[SubtitleString[i]].width + RecapSpacing;
			}
		}
	}
}

static void DisplaySubtitleThing_r(SubtitleThing *a1, const char *a2);
static Trampoline DisplaySubtitleThing_t(0x40E570, 0x40E575, DisplaySubtitleThing_r);
static void __cdecl DisplaySubtitleThing_r(SubtitleThing *a1, const char *a2)
{
	/*
	07 - center
	09 - tab
	20 - space
	0A - line break
	FFFFFF81 FFFFFFA1 - J square
	//40D7DA - draw subtitle texture
	*/
	auto original = reinterpret_cast<decltype(DisplaySubtitleThing_r)*>(DisplaySubtitleThing_t.Target());
	original(a1, a2);
	//PrintDebug("Subtitle: %s\n", a2);
	ParseSubtitle(a2);
}

static void DisplayRecapThing_r(ObjectMaster *a1);
static Trampoline DisplayRecapThing_t(0x642300, 0x642306, DisplayRecapThing_r);
static void __cdecl DisplayRecapThing_r(ObjectMaster *a1)
{
	EntityData1 *SubtitleEntity = a1->Data1;
	auto original = reinterpret_cast<decltype(DisplayRecapThing_r)*>(DisplayRecapThing_t.Target());
	original(a1);
	RecapScreenY = SubtitleEntity->Position.y;
	//PrintDebug("Ass: %f and %f", SubtitleEntity->Position.x, SubtitleEntity->Position.y);
}

int FindNewlineCharacter(const char* string)
{
	int result = -1;
	for (unsigned int i = 0; i < strlen(string); i++)
	{
		if (string[i] == 0x0A) result = i;
	}
	return result;
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
	for (int i = 0; i < LengthOfArray(FontCharacterData); i++)
	{
		PrintDebug("[%d]\n", i);
		PrintDebug("Width=%d\n", FontCharacterData[i].width);
		PrintDebug("OffsetX=%d\n", FontCharacterData[i].offset_x);
		PrintDebug("OffsetY=%d\n", FontCharacterData[i].offset_y);
		PrintDebug("\n");
	}
}

void DrawSubtitleHook(NJS_SPRITE *sp, Int n, Float pri, NJD_SPRITE attr, QueuedModelFlagsB queue_flags)
{
	float OldOffsetX = 0;
	int NewlinePosition = FindNewlineCharacter(SubtitleString);
	NJS_SPRITE SubtitleCharacterSprite = { { 0, 0, 0 }, 0.4f, 0.4f, 0, &SubtitleTexlist, SubtitleFont };
	SubtitleCharacterSprite.p.x = sp->p.x;
	SubtitleCharacterSprite.p.y = sp->p.y;
	njSetTexture(&SubtitleTexlist);
	//Set font color
	SetMaterialAndSpriteColor_Float(GlobalSpriteColor.a, SubtitleFontColorR / 255.0f, SubtitleFontColorG / 255.0f, SubtitleFontColorB / 255.0f);
	//Draw the whole string if there is no newline character
	if (NewlinePosition == -1)
	{
		//I guess both 0x07 and 0x09 count as centering when it's one line?
		if (SubtitleString[0] == 0x09 || SubtitleString[0] == 0x07) OldOffsetX = 320.0f - (SubtitleCharacterCount / 2.0f)*32.0f*SubtitleCharacterSprite.sx;
		//Padding for controls menu
		else OldOffsetX = 2.0f * (64.0f + SubtitleSpacing)*SubtitleCharacterSprite.sx;
		for (int i = 0; i < SubtitleCharacterCount; i++)
		{
			if (i == 0) SubtitleCharacterSprite.p.x = OldOffsetX;
			else SubtitleCharacterSprite.p.x = OldOffsetX + SubtitleCharacterSprite.sx * (FontCharacterData[SubtitleString[max(0, i - 1)] & 0xFF].width + SubtitleSpacing + FontCharacterData[SubtitleString[i] & 0xFF].offset_x);
			SubtitleCharacterSprite.p.y = sp->p.y + SubtitleCharacterSprite.sx * (FontCharacterData[SubtitleString[i] & 0xFF].offset_y);
			njDrawSprite2D_DrawNow(&SubtitleCharacterSprite, SubtitleString[i] & 0xFF, -1000.0f, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
			OldOffsetX = SubtitleCharacterSprite.p.x;
		}
	}
	else
	{
		//Draw first line
		//Center if the tab character is found
		if (SubtitleString[0] == 0x09) OldOffsetX = 320.0f - (NewlinePosition / 2.0f)*32.0f*SubtitleCharacterSprite.sx;
		//Center by the longer line if the bel character is found OR APPARENTLY IF ANYTHING
		else //if (SubtitleString[0] == 0x07) 
		{
			//Center by the second line
			if ((SubtitleCharacterCount - NewlinePosition) > NewlinePosition) OldOffsetX = 320.0f - ((SubtitleCharacterCount - NewlinePosition) / 2.0f)*32.0f*SubtitleCharacterSprite.sx;
			//Center by the first line
			else OldOffsetX = 320.0f - (NewlinePosition / 2.0f)*32.0f*SubtitleCharacterSprite.sx;
		}
		for (int i = 0; i < NewlinePosition; i++)
		{
			if (i == 0) SubtitleCharacterSprite.p.x = OldOffsetX;
			else
			{
				SubtitleCharacterSprite.p.x = OldOffsetX + SubtitleCharacterSprite.sx * (FontCharacterData[SubtitleString[max(0, i - 1)] & 0xFF].width + SubtitleSpacing + FontCharacterData[SubtitleString[i] & 0xFF].offset_x);
			}
			SubtitleCharacterSprite.p.y = sp->p.y + SubtitleCharacterSprite.sx * (FontCharacterData[SubtitleString[i] & 0xFF].offset_y);
			njDrawSprite2D_DrawNow(&SubtitleCharacterSprite, SubtitleString[i] & 0xFF, -1000.0f, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
			OldOffsetX = SubtitleCharacterSprite.p.x;
		}
		//Draw second line
		//Center if the tab character is found
		if (SubtitleString[NewlinePosition + 1] == 0x09) OldOffsetX = 320.0f - ((SubtitleCharacterCount - NewlinePosition) / 2.0f)*32.0f*SubtitleCharacterSprite.sx;
		//Center by the longer line if the bel character is found OR APPARENTLY IF ANYTHING
		else //if (SubtitleString[NewlinePosition + 1] == 0x07 || SubtitleString[0] == 0x07)
		{
			//Center by the second line
			if ((SubtitleCharacterCount - NewlinePosition) > NewlinePosition) OldOffsetX = 320.0f - ((SubtitleCharacterCount - NewlinePosition) / 2.0f)*32.0f*SubtitleCharacterSprite.sx;
			//Center by the first line
			else OldOffsetX = 320.0f - (NewlinePosition / 2.0f)*32.0f*SubtitleCharacterSprite.sx;
		}
		for (int i = 0; i < SubtitleCharacterCount - NewlinePosition; i++)
		{
			if (i == 0) SubtitleCharacterSprite.p.x = OldOffsetX;
			else
			{
				SubtitleCharacterSprite.p.x = OldOffsetX + SubtitleCharacterSprite.sx * (FontCharacterData[SubtitleString[max(0, NewlinePosition + i - 1)] & 0xFF].width + SubtitleSpacing + FontCharacterData[SubtitleString[NewlinePosition + i] & 0xFF].offset_x);
			}
			SubtitleCharacterSprite.p.y = sp->p.y + 26 + SubtitleCharacterSprite.sx * (FontCharacterData[SubtitleString[NewlinePosition + i] & 0xFF].offset_y);
			njDrawSprite2D_DrawNow(&SubtitleCharacterSprite, SubtitleString[NewlinePosition + i] & 0xFF, -1000.0f, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
			OldOffsetX = SubtitleCharacterSprite.p.x;
		}
	}
}

void LoadFontdata(const IniFile *config)
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
	for (int i = 0; i < LengthOfArray(FontCharacterData); i++)
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

void DrawRecapTextHook()
{
	/*for (int i = 0; i < NumberOfRecapLines; i++)
	{
		PrintDebug("Item%d: %s\n", i, RecapLineArray[i].LineString);
	}*/
	float ResolutionScale = (float)VerticalResolution / 480.0f;
	float FontAlpha = 1.0f;
	float OldOffsetX = 0;
	NJS_SPRITE SubtitleCharacterSprite = { { 0, 0, 0 }, 0.4f, 0.4f, 0, &SubtitleTexlist, SubtitleFont };
	njSetTexture(&SubtitleTexlist);
	//Set up scaling
	SubtitleCharacterSprite.sx = 0.4f*ResolutionScale;
	SubtitleCharacterSprite.sy = 0.4f*ResolutionScale;
	for (int u = 0; u < NumberOfRecapLines; u++)
	{
		//PrintDebug("Length: %i\n", RecapLineArray[u].LineLength);
		//Calculate centering
		if (RecapLineArray[u].LineString[0] == 0x07 || RecapLineArray[u].LineString[0] == 0x09) OldOffsetX = ((float)HorizontalResolution/2.0f) - SubtitleCharacterSprite.sx *(RecapLineArray[u].LineLength / 2);
		else OldOffsetX = 0;
		for (int i = 0; i < strlen(RecapLineArray[u].LineString); i++)
		{
			//Calculate X position
			if (i == 0) SubtitleCharacterSprite.p.x = OldOffsetX;
			else SubtitleCharacterSprite.p.x = OldOffsetX + SubtitleCharacterSprite.sx * (FontCharacterData[RecapLineArray[u].LineString[max(0, i - 1)] & 0xFF].width + RecapSpacing + FontCharacterData[RecapLineArray[u].LineString[i] & 0xFF].offset_x);
			//Calculate Y position
			SubtitleCharacterSprite.p.y = (u * 40 + 118 + RecapScreenY)*ResolutionScale + SubtitleCharacterSprite.sx * (FontCharacterData[RecapLineArray[u].LineString[i] & 0xFF].offset_y);
			//Set font color based on Y position
				if (SubtitleCharacterSprite.p.y <= 180 * ResolutionScale)
				{
					FontAlpha = max(0.0f, (SubtitleCharacterSprite.p.y / ResolutionScale - 100.0f) / 80.0f);
				}
				else if (SubtitleCharacterSprite.p.y >= 300 * ResolutionScale)
				{
					FontAlpha = max(0.0f, (60 + (300.0f - SubtitleCharacterSprite.p.y / ResolutionScale)) / 60.0f);
				}
				if (SubtitleCharacterSprite.p.y < 300 * ResolutionScale && SubtitleCharacterSprite.p.y > 180 * ResolutionScale) FontAlpha = 1.0f;
			SetMaterialAndSpriteColor_Float(max(0, FontAlpha - GlobalRecapAlphaForFadeout), (1.0f - GlobalRecapAlphaForFadeout)*RecapFontColorR/255.0f, (1.0f - GlobalRecapAlphaForFadeout)*RecapFontColorG / 255.0f, (1.0f - GlobalRecapAlphaForFadeout)*RecapFontColorB / 255.0f);
			//Draw
			njDrawSprite2D_ForcePriority(&SubtitleCharacterSprite, RecapLineArray[u].LineString[i] & 0xFF, 1000.0f, NJD_SPRITE_ALPHA | NJD_SPRITE_COLOR);
			OldOffsetX = SubtitleCharacterSprite.p.x;
		}
	}	
}

void RecapStart(SubtitleThing *a1)
{
	//PrintDebug("Recap start\n");
	if (ListenToRecap != 2) ListenToRecap = 1;
	DoTextThing_Start(a1);
}

void RecapStop(SubtitleThing *a1)
{
	//PrintDebug("Recap stop\n");
	if (ListenToRecap != 2)
	{
		ListenToRecap = 2;
	}
	DoTextThing_Stop(a1);
}

void LoadPVMHook_Recap(const char *filename, NJS_TEXLIST *texlist)
{
	LoadPVM(filename, texlist);
	LoadPVM("SUBTITLE", &SubtitleTexlist);
}

extern "C"
{
	__declspec(dllexport) ModInfo SADXModInfo = { ModLoaderVer };
	__declspec(dllexport) void __cdecl Init(const char *path, const HelperFunctions &helperFunctions)
	{
		char pathbuf[MAX_PATH];
		HMODULE GoalRing = GetModuleHandle(L"GoalRing");
		//Warnings
		if (helperFunctions.Version < 6)
		{
			MessageBox(WindowHandle,
				L"Mod Loader out of date. HD GUI requires API version 6 or newer.",
				L"HD GUI error: Mod loader out of date", MB_OK | MB_ICONERROR);
			return;
		}
		//Subtitle hooks
		WriteCall((void*)0x40D7DA, DrawSubtitleHook);
		WriteCall((void*)0x643DBA, LoadPVMHook_Recap);
		WriteCall((void*)0x642363, RecapStart);
		WriteCall((void*)0x6423EE, RecapStop);
		WriteCall((char*)0x642427, DrawRecapTextHook);
		//Add subtitle texlist to common object textures
		helperFunctions.RegisterCommonObjectPVM(SubtitlePVMEntry);
		//Load fontdata settings
		const std::string s_path(path);
		const std::string s_config_ini(helperFunctions.GetReplaceablePath("SYSTEM\\fontdata.ini"));
		const IniFile *const config = new IniFile(s_config_ini);
		LoadFontdata(config);
		delete config;
		//Fix green rectangle in tutorials
		WriteCall((void*)0x64393E, GreenRect_Wrapper);
		//Fix random ring icon
		WriteData((NJS_TEXANIM**)0x4C03FF, &RandomRingIconPart_TEXANIM);
		//Various fixes
		WriteCall((void*)0x00457F2F, DrawSprite_Hook);
		WriteCall((void*)0x00504DC4, HelpAvaSquareThing);
		WriteCall((void*)0x0050717E, ScaleCharselJapaneseText_LikeSeriouslyWTF);
		WriteCall((void*)0x005070E2, ScaleCharselJapaneseText_LikeSeriouslyWTF);
		WriteData<1>((char*)0x0091C00C, 0i8); //You win/lose text alignment
		WriteData<1>((char*)0x0091C00E, 0i8); //You win/lose text alignment
		WriteData<1>((char*)0x0091C020, 0i8); //You win/lose text alignment
		WriteData<1>((char*)0x0091C022, 0i8); //You win/lose text alignment
		WriteData<1>((char*)0x0091C034, 0i8); //You win/lose text alignment
		WriteData<1>((char*)0x0091C048, 0i8); //You win/lose text alignment
		ReplacePVMX("ENDBG_AMY_0");
		ReplacePVMX("ENDBG_AMY_1");
		ReplacePVMX("ENDBG_AMY_2");
		ReplacePVMX("ENDBG_BIG_0");
		ReplacePVMX("ENDBG_BIG_1");
		ReplacePVMX("ENDBG_BIG_2");
		ReplacePVMX("ENDBG_E102_0");
		ReplacePVMX("ENDBG_E102_1");
		ReplacePVMX("ENDBG_E102_2");
		ReplacePVMX("ENDBG_KNUCKLES_0");
		ReplacePVMX("ENDBG_KNUCKLES_1");
		ReplacePVMX("ENDBG_KNUCKLES_2");
		ReplacePVMX("ENDBG_SONIC_0");
		ReplacePVMX("ENDBG_SONIC_1");
		ReplacePVMX("ENDBG_SONIC_2");
		ReplacePVMX("ENDBG_SUPERSONIC_0");
		ReplacePVMX("ENDBG_SUPERSONIC_1");
		ReplacePVMX("ENDBG_SUPERSONIC_2");
		ReplacePVMX("ENDBG_SUPERSONIC_3");
		ReplacePVMX("ENDBG_TAILS_0");
		ReplacePVMX("ENDBG_TAILS_1");
		ReplacePVMX("ENDBG_TAILS_2");
		ReplacePVMX("ADV_WINDOW");
		ReplacePVMX("AVA_BACK");
		ReplacePVMX("AVA_CHSEL");
		ReplacePVMX("AVA_CHSEL_E");
		ReplacePVMX("AVA_CSR");
		ReplacePVMX("AVA_DLG");
		ReplacePVMX("AVA_DLG_E");
		ReplacePVMX("AVA_EMBLEMVIEW");
		ReplacePVMX("AVA_EMBLEMVIEW_E");
		ReplacePVMX("AVA_EMBLEM");
		ReplacePVMX("AVA_FILESEL");
		ReplacePVMX("AVA_FILESEL_E");
		ReplacePVMX("AVA_FSDLG");
		ReplacePVMX("AVA_FSDLG_E");
		ReplacePVMX("AVA_METAL_SONIC");
		ReplacePVMX("AVA_NEW16NO");
		ReplacePVMX("AVA_OPTION");
		ReplacePVMX("AVA_OPTION_E");
		ReplacePVMX("AVA_SAN");
		ReplacePVMX("AVA_SNDTEST");
		ReplacePVMX("AVA_SNDTEST_E");
		ReplacePVMX("AVA_SQUARE");
		ReplacePVMX("AVA_STNAM");
		ReplacePVMX("AVA_STNAM_E");
		ReplacePVMX("AVA_SUUJI");
		//ReplacePVMX("AVA_TITLE_BACK"); - not used
		ReplacePVMX("AVA_TITLE_BACK_E");
		ReplacePVMX("AVA_TITLE");
		ReplacePVMX("AVA_TITLE_E");
		ReplacePVMX("AVA_TRIALACTSEL");
		ReplacePVMX("AVA_TRIALACTSEL_E");
		ReplacePVMX("AVA_VMSSEL");
		ReplacePVMX("AVA_VMSSEL_E");
		ReplacePVMX("BOARD_SCORE");
		ReplacePVMX("B_CHNAM");
		ReplacePVMX("B_CHNAM_E");
		ReplacePVMX("CHAOS_LIFEGAUGE");
		ReplacePVMX("CON_REGULAR");
		ReplacePVMX("CON_REGULAR_E");
		ReplacePVMX("E102TIME");
		ReplacePVMX("EMBLEM");
		ReplacePVMX("EXTRA");
		ReplacePVMX("ENDBG_LOGO");
		ReplacePVMX("FISHING");
		ReplacePVMX("GAMEOVER");
		ReplacePVMX("GAMEOVER_E");
		//ReplacePVMX("GG_TEXLIST_US");
		ReplacePVMX("KNU_EFF");
		ReplacePVMX("MAP_EC_A");
		ReplacePVMX("MAP_EC_B");
		ReplacePVMX("MAP_EC_H");
		ReplacePVMX("MAP_ICON");
		ReplacePVMX("MAP_MR_A");
		ReplacePVMX("MAP_MR_J");
		ReplacePVMX("MAP_MR_S");
		ReplacePVMX("MAP_PAST_E");
		ReplacePVMX("MAP_PAST_S");
		ReplacePVMX("MAP_SS");
		ReplacePVMX("MILESRACE");
		ReplacePVMX("MISSION_TUTO");
		ReplacePVMX("MIS_C_EN");
		ReplacePVMX("MIS_C_J");
		ReplacePVMX("MIS_P");
		ReplacePVMX("M_CHNAM");
		ReplacePVMX("OBJ_MINI_CART");
		ReplacePVMX("OBJ_REGULAR");
		ReplacePVMX("PRESSSTART");
		ReplacePVMX("SCORE_ACT");
		ReplacePVMX("SCORE_ACT_E");
		ReplacePVMX("SCORE_BACK");
		ReplacePVMX("SCORE_BOARD");
		ReplacePVMX("SCORE_BOARD_E");
		ReplacePVMX("SCORE_BOSS");
		ReplacePVMX("SCORE_BOSS_E");
		ReplacePVMX("SCORE_CART");
		ReplacePVMX("SCORE_CART_E");
		ReplacePVMX("SCORE_MOLE");
		ReplacePVMX("SCORE_MOLE_E");
		ReplacePVMX("SCORE_RESULT");
		ReplacePVMX("SCORE_RESULT_E");
		ReplacePVMX("SCORE_SHOOT");
		ReplacePVMX("SCORE_SHOOT_E");
		ReplacePVMX("SEGALOGO");
		ReplacePVMX("SEGALOGO_E");
		ReplacePVMX("SHOOTING0");
		ReplacePVMX("SMRYBG_AMY");
		ReplacePVMX("SMRYBG_BIG");
		ReplacePVMX("SMRYBG_E102");
		ReplacePVMX("SMRYBG_KNUCKLES");
		ReplacePVMX("SMRYBG_SONIC");
		ReplacePVMX("SMRYBG_SUPERSONIC");
		ReplacePVMX("SMRYBG_TAILS");
		ReplacePVMX("TX_CHNAM");
		ReplacePVMX("TX_CHNAM_E");
		//Common PVRs
		ReplacePNG_Common("ABC_TXT");
		ReplacePNG_Common("HYOJI_BALLS_E");
		ReplacePNG_Common("HYOJI_EMBLEM0");
		ReplacePNG_Common("HYOJI_EMBLEM1");
		ReplacePNG_Common("B32ASCII");
		ReplacePNG_Common("B32ASCII_J");
		ReplacePNG_Common("STAFFROLL_TXT");
		ReplacePNG_Common("ST_064S_LOCKA");
		ReplacePNG_Common("ST_064S_LOCKB");
		ReplacePNG_Common("ST_064S_LOCKC");
		ReplacePNG_Common("ST_064S_SCORE");
		ReplacePNG_Common("ST_128S_HPGEJI");
		//English stage PVRs
		ReplacePNG_StageE("A_STAGE01_E");
		ReplacePNG_StageE("A_STAGE02_E");
		ReplacePNG_StageE("A_STAGE03_E");
		ReplacePNG_StageE("B_STAGE01_E");
		ReplacePNG_StageE("B_STAGE02_E");
		ReplacePNG_StageE("B_STAGE03_E");
		ReplacePNG_StageE("B_STAGE04_E");
		ReplacePNG_StageE("E_STAGE01_E");
		ReplacePNG_StageE("E_STAGE02_E");
		ReplacePNG_StageE("E_STAGE03_E");
		ReplacePNG_StageE("E_STAGE04_E");
		ReplacePNG_StageE("E_STAGE05_E");
		ReplacePNG_StageE("K_STAGE01_E");
		ReplacePNG_StageE("K_STAGE02_E");
		ReplacePNG_StageE("K_STAGE03_E");
		ReplacePNG_StageE("K_STAGE04_E");
		ReplacePNG_StageE("K_STAGE05_E");
		ReplacePNG_StageE("M_STAGE01_E");
		ReplacePNG_StageE("M_STAGE02_E");
		ReplacePNG_StageE("M_STAGE03_E");
		ReplacePNG_StageE("M_STAGE04_E");
		ReplacePNG_StageE("M_STAGE05_E");
		ReplacePNG_StageE("ST_STAGE01_E");
		ReplacePNG_StageE("ST_STAGE02_E");
		ReplacePNG_StageE("ST_STAGE03_E");
		ReplacePNG_StageE("ST_STAGE04_E");
		ReplacePNG_StageE("ST_STAGE05_E");
		ReplacePNG_StageE("S_STAGE01_E");
		ReplacePNG_StageE("S_STAGE02_E");
		ReplacePNG_StageE("S_STAGE03_E");
		ReplacePNG_StageE("S_STAGE04_E");
		ReplacePNG_StageE("S_STAGE05_E");
		ReplacePNG_StageE("S_STAGE06_E");
		ReplacePNG_StageE("S_STAGE07_E");
		ReplacePNG_StageE("S_STAGE08_E");
		ReplacePNG_StageE("S_STAGE09_E");
		ReplacePNG_StageE("S_STAGE10_E");
		ReplacePNG_StageE("T_EGGCARRIER_E");
		ReplacePNG_StageE("T_MISTICRUIN_E");
		ReplacePNG_StageE("T_STATIONSQUARE_E");
		//Japanese stage PVRs
		ReplacePNG_StageJ("A_STAGE01");
		ReplacePNG_StageJ("A_STAGE02");
		ReplacePNG_StageJ("A_STAGE03");
		ReplacePNG_StageJ("B_STAGE01");
		ReplacePNG_StageJ("B_STAGE02");
		ReplacePNG_StageJ("B_STAGE03");
		ReplacePNG_StageJ("B_STAGE04");
		ReplacePNG_StageJ("E_STAGE01");
		ReplacePNG_StageJ("E_STAGE02");
		ReplacePNG_StageJ("E_STAGE03");
		ReplacePNG_StageJ("E_STAGE04");
		ReplacePNG_StageJ("E_STAGE05");
		ReplacePNG_StageJ("K_STAGE01");
		ReplacePNG_StageJ("K_STAGE02");
		ReplacePNG_StageJ("K_STAGE03");
		ReplacePNG_StageJ("K_STAGE04");
		ReplacePNG_StageJ("K_STAGE05");
		ReplacePNG_StageJ("M_STAGE01");
		ReplacePNG_StageJ("M_STAGE02");
		ReplacePNG_StageJ("M_STAGE03");
		ReplacePNG_StageJ("M_STAGE04");
		ReplacePNG_StageJ("M_STAGE05");
		ReplacePNG_StageJ("ST_STAGE01");
		ReplacePNG_StageJ("ST_STAGE02");
		ReplacePNG_StageJ("ST_STAGE03");
		ReplacePNG_StageJ("ST_STAGE04");
		ReplacePNG_StageJ("ST_STAGE05");
		ReplacePNG_StageJ("S_STAGE01");
		ReplacePNG_StageJ("S_STAGE02");
		ReplacePNG_StageJ("S_STAGE03");
		ReplacePNG_StageJ("S_STAGE04");
		ReplacePNG_StageJ("S_STAGE05");
		ReplacePNG_StageJ("S_STAGE06");
		ReplacePNG_StageJ("S_STAGE07");
		ReplacePNG_StageJ("S_STAGE08");
		ReplacePNG_StageJ("S_STAGE09");
		ReplacePNG_StageJ("S_STAGE10");
		ReplacePNG_StageJ("T_EGGCARRIER");
		ReplacePNG_StageJ("T_MISTICRUIN");
		ReplacePNG_StageJ("T_STATIONSQUARE");
		//English mission PVRs
		ReplacePNG_MissionE("MISSION_A_BALRING_E");
		ReplacePNG_MissionE("MISSION_A_BALZERO_E");
		ReplacePNG_MissionE("MISSION_A_FIN_E");
		ReplacePNG_MissionE("MISSION_A_HOT_E");
		ReplacePNG_MissionE("MISSION_A_TWIN_E");
		ReplacePNG_MissionE("MISSION_BIG_1K_E");
		ReplacePNG_MissionE("MISSION_BIG_2K_E");
		ReplacePNG_MissionE("MISSION_BIG_FROG_E");
		ReplacePNG_MissionE("MISSION_G_103RING_E");
		ReplacePNG_MissionE("MISSION_G_103_E");
		ReplacePNG_MissionE("MISSION_G_104RING_E");
		ReplacePNG_MissionE("MISSION_G_104_E");
		ReplacePNG_MissionE("MISSION_G_105RING_E");
		ReplacePNG_MissionE("MISSION_G_105_E");
		ReplacePNG_MissionE("MISSION_G_EME_E");
		ReplacePNG_MissionE("MISSION_G_FIN_E");
		ReplacePNG_MissionE("MISSION_G_FROGRING_E");
		ReplacePNG_MissionE("MISSION_G_FROG_E");
		ReplacePNG_MissionE("MISSION_G_SONICDRING_E");
		ReplacePNG_MissionE("MISSION_G_HOT_E");
		ReplacePNG_MissionE("MISSION_G_RED_E");
		ReplacePNG_MissionE("MISSION_G_SONICD_E");
		ReplacePNG_MissionE("MISSION_G_WIN_E");
		ReplacePNG_MissionE("MISSION_K_1MIN_E");
		ReplacePNG_MissionE("MISSION_K_2MIN_E");
		ReplacePNG_MissionE("MISSION_K_3EME_E");
		ReplacePNG_MissionE("MISSION_K_NOHINT_E");
		ReplacePNG_MissionE("MISSION_S_EGGC_E");
		ReplacePNG_MissionE("MISSION_S_EMECASINO_E");
		ReplacePNG_MissionE("MISSION_S_EMESNOW_E");
		ReplacePNG_MissionE("MISSION_S_EMEWIND_E");
		ReplacePNG_MissionE("MISSION_S_FEGG_E");
		ReplacePNG_MissionE("MISSION_S_ISEKI_E");
		ReplacePNG_MissionE("MISSION_S_TAILS_E");
		ReplacePNG_MissionE("MISSION_T_EMECASINO_E");
		ReplacePNG_MissionE("MISSION_T_EMESNOW_E");
		ReplacePNG_MissionE("MISSION_T_EMEWIND_E");
		ReplacePNG_MissionE("MISSION_T_FASTEGG_E");
		ReplacePNG_MissionE("MISSION_T_FASTSONIC_E");
		ReplacePNG_MissionE("MISSION_T_MISS_E");
		ReplacePNG_MissionE("MISSION_T_RINGEGG_E");
		ReplacePNG_MissionE("MISSION_T_RINGSONIC_E");
		//Japanese mission PVRs
		ReplacePNG_MissionJ("MISSION_A_AFIN");
		ReplacePNG_MissionJ("MISSION_A_AHOT");
		ReplacePNG_MissionJ("MISSION_A_ATWI");
		ReplacePNG_MissionJ("MISSION_A_BALMIN");
		ReplacePNG_MissionJ("MISSION_A_BALRING");
		ReplacePNG_MissionJ("MISSION_A_BALZERO");
		ReplacePNG_MissionJ("MISSION_BIG_1K");
		ReplacePNG_MissionJ("MISSION_BIG_2K");
		ReplacePNG_MissionJ("MISSION_BIG_FROG");
		ReplacePNG_MissionJ("MISSION_G_103");
		ReplacePNG_MissionJ("MISSION_G_103RING");
		ReplacePNG_MissionJ("MISSION_G_103SEC");
		ReplacePNG_MissionJ("MISSION_G_104");
		ReplacePNG_MissionJ("MISSION_G_104RING");
		ReplacePNG_MissionJ("MISSION_G_105");
		ReplacePNG_MissionJ("MISSION_G_105RING");
		ReplacePNG_MissionJ("MISSION_G_105SEC");
		ReplacePNG_MissionJ("MISSION_G_AEME");
		ReplacePNG_MissionJ("MISSION_G_AFIN");
		ReplacePNG_MissionJ("MISSION_G_AHOT");
		ReplacePNG_MissionJ("MISSION_G_ARED");
		ReplacePNG_MissionJ("MISSION_G_AWIN");
		ReplacePNG_MissionJ("MISSION_G_BEME");
		ReplacePNG_MissionJ("MISSION_G_BFIN");
		ReplacePNG_MissionJ("MISSION_G_BHOT");
		ReplacePNG_MissionJ("MISSION_G_BRED");
		ReplacePNG_MissionJ("MISSION_G_BWIN");
		ReplacePNG_MissionJ("MISSION_G_FROG");
		ReplacePNG_MissionJ("MISSION_G_FROGRING");
		ReplacePNG_MissionJ("MISSION_G_FROGSEC");
		ReplacePNG_MissionJ("MISSION_G_SONICD");
		ReplacePNG_MissionJ("MISSION_G_SONICDRING");
		ReplacePNG_MissionJ("MISSION_G_SONICDSEC");
		ReplacePNG_MissionJ("MISSION_K_1MIN");
		ReplacePNG_MissionJ("MISSION_K_2MIN");
		ReplacePNG_MissionJ("MISSION_K_3EME");
		ReplacePNG_MissionJ("MISSION_K_NOHINT");
		ReplacePNG_MissionJ("MISSION_S_BOXMIN");
		ReplacePNG_MissionJ("MISSION_S_EGGC");
		ReplacePNG_MissionJ("MISSION_S_EMECASINO");
		ReplacePNG_MissionJ("MISSION_S_EMESNOW");
		ReplacePNG_MissionJ("MISSION_S_EMEWIND");
		ReplacePNG_MissionJ("MISSION_S_FEGG");
		ReplacePNG_MissionJ("MISSION_S_ISEKI");
		ReplacePNG_MissionJ("MISSION_S_TAILS");
		ReplacePNG_MissionJ("MISSION_T_EMECASINO");
		ReplacePNG_MissionJ("MISSION_T_EMESNOW");
		ReplacePNG_MissionJ("MISSION_T_EMEWIND");
		ReplacePNG_MissionJ("MISSION_T_FASTEGG");
		ReplacePNG_MissionJ("MISSION_T_FASTSONIC");
		ReplacePNG_MissionJ("MISSION_T_MISS");
		ReplacePNG_MissionJ("MISSION_T_RINGEGG");
		ReplacePNG_MissionJ("MISSION_T_RINGSONIC");
		if (GoalRing == nullptr)
		{
			//Japanese
			ReplacePNG_MissionJ("MISSION_S_2MIN");
			ReplacePNG_MissionJ("MISSION_S_2MINH");
			ReplacePNG_MissionJ("MISSION_S_3MIN");
			ReplacePNG_MissionJ("MISSION_S_4MIN");
			ReplacePNG_MissionJ("MISSION_S_4MINH");
			ReplacePNG_MissionJ("MISSION_S_5MIN");
			ReplacePNG_MissionJ("MISSION_S_BOX");
			ReplacePNG_MissionJ("MISSION_S_RINGBOX");
			ReplacePNG_MissionJ("MISSION_T_BOX");
			//English
			ReplacePNG_MissionE("MISSION_S_BOX_E");
			ReplacePNG_MissionE("MISSION_S_BOX25MIN_E");
			ReplacePNG_MissionE("MISSION_S_BOX2MIN_E");
			ReplacePNG_MissionE("MISSION_S_BOX3MIN_E");
			ReplacePNG_MissionE("MISSION_S_BOX45MIN_E");
			ReplacePNG_MissionE("MISSION_S_BOX4MIN_E");
			ReplacePNG_MissionE("MISSION_S_BOX5MIN_E");
			ReplacePNG_MissionE("MISSION_S_RINGBOX_E");
			ReplacePNG_MissionE("MISSION_T_BOX_E");
		}
		else
		{
			//Japanese
			ReplacePNG_GoalRing("MISSION_S_2MIN");
			ReplacePNG_GoalRing("MISSION_S_2MINH");
			ReplacePNG_GoalRing("MISSION_S_3MIN");
			ReplacePNG_GoalRing("MISSION_S_4MIN");
			ReplacePNG_GoalRing("MISSION_S_4MINH");
			ReplacePNG_GoalRing("MISSION_S_5MIN");
			ReplacePNG_GoalRing("MISSION_S_BOX");
			ReplacePNG_GoalRing("MISSION_S_RINGBOX");
			ReplacePNG_GoalRing("MISSION_T_BOX");
			//English
			ReplacePNG_GoalRing("MISSION_S_BOX_E");
			ReplacePNG_GoalRing("MISSION_S_BOX25MIN_E");
			ReplacePNG_GoalRing("MISSION_S_BOX2MIN_E");
			ReplacePNG_GoalRing("MISSION_S_BOX3MIN_E");
			ReplacePNG_GoalRing("MISSION_S_BOX45MIN_E");
			ReplacePNG_GoalRing("MISSION_S_BOX4MIN_E");
			ReplacePNG_GoalRing("MISSION_S_BOX5MIN_E");
			ReplacePNG_GoalRing("MISSION_S_RINGBOX_E");
			ReplacePNG_GoalRing("MISSION_T_BOX_E");
		}
		//Screen fade fixes
		f480_Fixed = 1.0f + VerticalResolution;
		f640_Fixed = 1.0f + HorizontalResolution;
		WriteData((float**)0x00433385, &f480_Fixed); //Screen fade resolution
		WriteData((float**)0x004333A6, &f640_Fixed); //Screen fade resolution
		WriteData((float*)0x004333A0, -1.0f); //Screen fade for tutorials
		WriteData((float*)0x004333AE, -1.0f); //Screen fade for tutorials
		WriteCall((void*)0x0042BF52, ScreenFadeFix);
		WriteData<5>((char*)0x0040BF27, 0x90); //Disable "Now saving..."
		WriteData<5>((void*)0x0040BE0D, 0x90); //Disable "Now loading..."
		WriteData<5>((void*)0x00503438, 0x90); //Disable "Now loading..."
		WriteData<5>((void*)0x0050346D, 0x90); //Disable "Now loading..."
		//Character select screen fixes
		WriteCall((void*)0x00511AD0, RetrievePlayerSelectStuff); //Player select text in character select screen
		WriteCall((void*)0x00511C76, RetrieveBottomThingStuff); //Bottom thing in character select screen
		WriteCall((void*)0x00511B3B, RenderShittyTextures); //Render stuff that refuses to render properly otherwise
		WriteCall((void*)0x00511A8B, DisplayScreenTexture_AlwaysTop); //Move the "Select your character" text to top
		WriteData<5>((void*)0x00511C18, 0x90); //Disable ZFunc stuff to prevent character model overlap issues
		//Shadow blending fixes
		WriteCall((void*)0x0050B584, DrawShadow_Hook);
		WriteCall((void*)0x00431D37, DrawShadow_Hook);
		WriteCall((void*)0x00506EFF, DrawShadow_Hook);
		WriteCall((void*)0x0050D8B3, DrawShadow_Hook);
		WriteCall((void*)0x0050B61A, DrawShadow_Hook); //Main menu (trial) shadow
		WriteCall((void*)0x00508FFD, DrawTexture_Hook); //Sound test icon
		WriteCall((void*)0x00509130, DrawTexture_Hook); //Sonic icon background
		WriteCall((void*)0x00509191, DrawTexture_Hook); //Sonic icon
		WriteCall((void*)0x00509439, DrawTexture_Hook); //Languages icon
		WriteCall((void*)0x0050952F, DrawTexture_Hook); //Rumble icon
		WriteCall((void*)0x0050782A, DrawTexture_Hook); //AVA_SAN triangle shadow
		((NJS_OBJECT*)0x10D7774)->basicdxmodel->mats[0].diffuse.color = 0xFFB2B2B2; //Question mark from Character Select
		((NJS_OBJECT*)0x10D7774)->basicdxmodel->mats[0].attr_texId = 10; //Question mark from Character Select
		ResizeTextureList(&OBJ_REGULAR_TEXLIST, 100); //Added DC ripple texture
		WriteCall((void*)0x005092A1, FileIcon_Hook); //File icon
		*(NJS_MODEL_SADX*)0x00989384 = attach_0019D298_2; //Switch
		*(NJS_MODEL_SADX*)0x008BBD84 = attach_0019D298; //Switch (pressed)
		*(NJS_MODEL_SADX*)0x008B8438 = attach_00199A4C; //Dash panel
		*(NJS_MODEL_SADX*)0x008C5D5C = attach_001A6F74; //Star panel
		*(NJS_MODEL_SADX*)0x008C9060 = attach_001AA1B8; //Checkpoint
		*(NJS_MODEL_SADX*)0x008B6010 = attach_00197698; //Normal spring
		*(NJS_MODEL_SADX*)0x008B5498 = attach_00196B5C; //Spring B
		*(NJS_MODEL_SADX*)0x008C6624 = attach_001A7820; //Spring H
		*(NJS_MODEL_SADX*)0x008BFEC8 = attach_001A127C; //Rocket platform
		*(NJS_MODEL_SADX*)0x008BE168 = attach_0019F5CC; //Balloon
		*(NJS_MODEL_SADX*)0x008BFEC8 = attach_001A127C; //Rocket platform
		*(NJS_OBJECT *)0x02982F44 = object_0003FA40; //Egg Carrier model
		*(NJS_OBJECT *)0x298A894 = object_00047110; //Hodai
		*(NJS_OBJECT *)0x2941B2C = object_0001342C; //Kirai
		((NJS_ACTION*)0x2996C74)->object = &object_0004AEE0; //Beam in Act 2
		*(NJS_OBJECT *)0x0298E7D0 = object_0004AEE0; //Beam in Act 2
		//Tutorial stuff
		//PVMs
		ReplacePVMX("TUTO_CMN");
		ReplacePVMX("TUTO_CMN_E");
		ReplacePVMX("TUTOMSG_AMY_E");
		ReplacePVMX("TUTOMSG_BIG_E");
		ReplacePVMX("TUTOMSG_E102_E");
		ReplacePVMX("TUTOMSG_KNUCKLES_E");
		ReplacePVMX("TUTOMSG_SONIC_E");
		ReplacePVMX("TUTOMSG_TAILS_E");
		ReplacePVMX("TUTOMSG_AMY");
		ReplacePVMX("TUTOMSG_AMY_F");
		ReplacePVMX("TUTOMSG_AMY_G");
		ReplacePVMX("TUTOMSG_AMY_S");
		ReplacePVMX("TUTOMSG_BIG");
		ReplacePVMX("TUTOMSG_BIG_F");
		ReplacePVMX("TUTOMSG_BIG_G");
		ReplacePVMX("TUTOMSG_BIG_S");
		ReplacePVMX("TUTOMSG_E102");
		ReplacePVMX("TUTOMSG_E102_F");
		ReplacePVMX("TUTOMSG_E102_G");
		ReplacePVMX("TUTOMSG_E102_S");
		ReplacePVMX("TUTOMSG_KNUCKLES");
		ReplacePVMX("TUTOMSG_KNUCKLES_F");
		ReplacePVMX("TUTOMSG_KNUCKLES_G");
		ReplacePVMX("TUTOMSG_KNUCKLES_S");
		ReplacePVMX("TUTOMSG_SONIC");
		ReplacePVMX("TUTOMSG_SONIC_F");
		ReplacePVMX("TUTOMSG_SONIC_G");
		ReplacePVMX("TUTOMSG_SONIC_S");
		ReplacePVMX("TUTOMSG_TAILS");
		ReplacePVMX("TUTOMSG_TAILS_F");
		ReplacePVMX("TUTOMSG_TAILS_G");
		ReplacePVMX("TUTOMSG_TAILS_S");
		WriteData((float**)0x0064328D, &PadManuXOffset_F);
		WriteData((float**)0x00643295, &PadManuXOffset_General);
		WriteData((float**)0x00643280, &PadManuXOffset_J);
		WriteData((float**)0x006432C6, &PadManuYOffset);
		WriteData((float**)0x006432E4, &PadManuYOffset2);
		WriteData((float**)0x006432D4, &PadManuYMultiplier);
		//Sonic
		//English
		TutoScreenSonic_E[0].BoxScaleX = 390;
		TutoScreenSonic_E[0].BoxScaleY = 144;
		TutoScreenSonic_E[0].BoxX = 210;
		TutoScreenSonic_E[4].BoxScaleY = 160;
		TutorialLayout_Sonic_Page1_E[1].XOffset = 136; //Rotate camera
		TutorialLayout_Sonic_Page1_E[1].YOffset = 0; //Rotate camera
		TutorialLayout_Sonic_Page1_E[2].XOffset = 136; //Maneuver character
		TutorialLayout_Sonic_Page1_E[2].YOffset = 24; //Maneuver character
		TutorialLayout_Sonic_Page1_E[4].XOffset = 136; //A button
		TutorialLayout_Sonic_Page1_E[4].YOffset = 64; //A button
		TutorialLayout_Sonic_Page1_E[4].TexID = 3;
		TutorialLayout_Sonic_Page1_E[5].TexID = 4;
		TutorialLayout_Sonic_Page1_E[5].XOffset = 136;
		TutorialLayout_Sonic_Page1_E[5].YOffset = 96;
		WriteData((__int16*)0x02BC3AE2, (__int16)2000); //Hide an extra item in controls page (all langs)
		//Japanese
		TutoScreenSonic_J[0].BoxScaleX = 400;
		TutoScreenSonic_J[0].BoxScaleY = 144;
		TutoScreenSonic_J[0].BoxX = 200;
		TutoScreenSonic_J[4].BoxScaleY = 160;
		TutorialLayout_Sonic_Page1_J[1].XOffset = 136; //Rotate camera
		TutorialLayout_Sonic_Page1_J[1].YOffset = 0; //Rotate camera
		TutorialLayout_Sonic_Page1_J[2].XOffset = 136; //Maneuver character
		TutorialLayout_Sonic_Page1_J[2].YOffset = 24; //Maneuver character
		TutorialLayout_Sonic_Page1_J[3].XOffset = 136; //A button
		TutorialLayout_Sonic_Page1_J[3].YOffset = 64; //A button
		TutorialLayout_Sonic_Page1_J[3].TexID = 3;
		TutorialLayout_Sonic_Page1_J[4].TexID = 4;
		TutorialLayout_Sonic_Page1_J[4].XOffset = 136;
		TutorialLayout_Sonic_Page1_J[4].YOffset = 96;
		//German
		TutoScreenSonic_G[0].BoxScaleX = 380;
		TutoScreenSonic_G[0].BoxScaleY = 144;
		TutoScreenSonic_G[0].BoxX = 220;
		TutoScreenSonic_G[3].BoxScaleY = 160;
		TutoScreenSonic_G[4].BoxScaleY = 160;
		//French
		TutoScreenSonic_F[4].BoxScaleY = 192;
		TutoScreenSonic_F[0].BoxScaleX = 425;
		TutoScreenSonic_F[0].BoxScaleY = 144;
		TutoScreenSonic_F[0].BoxX = 175;
		//Spanish
		TutoScreenSonic_S[0].BoxScaleX = 370;
		TutoScreenSonic_S[0].BoxScaleY = 144;
		TutoScreenSonic_S[0].BoxX = 230;
		TutoScreenSonic_S[4].BoxScaleY = 192;
		//Tails
		//English
		TutoScreenTails_E[0].BoxScaleX = 390;
		TutoScreenTails_E[0].BoxScaleY = 144;
		TutoScreenTails_E[0].BoxX = 210;
		TutoScreenTails_E[4].BoxScaleY = 160;
		TutorialLayout_SharedTailsKnucklesPage1_E[0].XOffset = 136; //Rotate camera
		TutorialLayout_SharedTailsKnucklesPage1_E[0].YOffset = 0; //Rotate camera
		TutorialLayout_SharedTailsKnucklesPage1_E[1].XOffset = 136; //Maneuver character
		TutorialLayout_SharedTailsKnucklesPage1_E[1].YOffset = 24; //Maneuver character
		TutorialLayout_SharedTailsKnucklesPage1_E[3].XOffset = 136; //A button
		TutorialLayout_SharedTailsKnucklesPage1_E[3].YOffset = 96; //A button
		TutorialLayout_Tails_Page1_E.XOffset = 136; //Tail attack
		TutorialLayout_Tails_Page1_E.YOffset = 64; //Tail attack
		//Japanese
		TutoScreenTails_J[0].BoxScaleX = 400;
		TutoScreenTails_J[0].BoxScaleY = 144;
		TutoScreenTails_J[0].BoxX = 200;
		TutoScreenTails_J[4].BoxScaleY = 160;
		TutorialLayout_SharedTailsKnucklesPage1_J[0].XOffset = 136; //Rotate camera
		TutorialLayout_SharedTailsKnucklesPage1_J[0].YOffset = 0; //Rotate camera
		TutorialLayout_SharedTailsKnucklesPage1_J[1].XOffset = 136; //Maneuver character
		TutorialLayout_SharedTailsKnucklesPage1_J[1].YOffset = 24; //Maneuver character
		TutorialLayout_SharedTailsKnucklesPage1_J[2].XOffset = 136; //A button
		TutorialLayout_SharedTailsKnucklesPage1_J[2].YOffset = 96; //A button
		TutorialLayout_Tails_Page1_J.XOffset = 136; //Tail attack
		TutorialLayout_Tails_Page1_J.YOffset = 64; //Tail attack
		//French
		TutoScreenTails_F[4].BoxScaleY = 192;
		TutoScreenTails_F[0].BoxScaleX = 425;
		TutoScreenTails_F[0].BoxScaleY = 144;
		TutoScreenTails_F[0].BoxX = 175;
		//German
		TutoScreenTails_G[0].BoxScaleX = 390;
		TutoScreenTails_G[0].BoxScaleY = 144;
		TutoScreenTails_G[0].BoxX = 210;
		TutoScreenTails_G[3].BoxScaleY = 128;
		TutoScreenTails_G[4].BoxScaleY = 160;
		TutoScreenTails_G[0].BoxScaleX = 380;
		TutoScreenTails_G[0].BoxScaleY = 144;
		TutoScreenTails_G[0].BoxX = 220;
		WriteData((__int16*)0x02BC3E9E, (__int16)2000); //Hide an extra item in controls page
		//Spanish
		TutoScreenTails_S[0].BoxScaleX = 370;
		TutoScreenTails_S[0].BoxScaleY = 144;
		TutoScreenTails_S[0].BoxX = 230;
		TutoScreenTails_S[4].BoxScaleY = 192;
		//Knuckles
		//English
		TutoScreenKnuckles_E[0].BoxScaleX = 390;
		TutoScreenKnuckles_E[0].BoxScaleY = 144;
		TutoScreenKnuckles_E[0].BoxX = 210;
		TutoScreenKnuckles_E[4].BoxX = 180;
		TutoScreenKnuckles_E[4].BoxScaleY = 128;
		TutoScreenKnuckles_E[4].BoxScaleX = 425;
		TutoScreenKnuckles_E[5].BoxScaleY = 160;
		TutorialLayout_Knuckles_Page1_E.XOffset = 136; //Punch attack
		TutorialLayout_Knuckles_Page1_E.YOffset = 64; //Punch attack
		WriteData<1>((char*)0x02BC4308, 0x03); //Number of items in Maximum Heat screen, remove unnecessary line
		//Japanese
		TutoScreenKnuckles_J[0].BoxScaleX = 400;
		TutoScreenKnuckles_J[0].BoxScaleY = 144;
		TutoScreenKnuckles_J[0].BoxX = 200;
		TutoScreenKnuckles_J[4].BoxX = 180;
		TutoScreenKnuckles_J[4].BoxScaleY = 128;
		TutoScreenKnuckles_J[4].BoxScaleX = 425;
		TutoScreenKnuckles_J[5].BoxScaleY = 160;
		TutorialLayout_Knuckles_Page1_J.XOffset = 136; //Punch attack
		TutorialLayout_Knuckles_Page1_J.YOffset = 64; //Punch attack
		//German
		TutoScreenKnuckles_G[0].BoxScaleX = 380;
		TutoScreenKnuckles_G[0].BoxScaleY = 144;
		TutoScreenKnuckles_G[0].BoxX = 220;
		TutoScreenKnuckles_G[2].BoxScaleY = 128;
		TutoScreenKnuckles_G[4].BoxX = 180;
		TutoScreenKnuckles_G[4].BoxScaleX = 420;
		TutoScreenKnuckles_G[4].BoxScaleY = 128;
		TutoScreenKnuckles_G[5].BoxScaleY = 160;
		WriteData((__int16*)0x02BC42E0, (__int16)2000); //Hide an extra item in Climbing page
		//French
		TutoScreenKnuckles_F[0].BoxScaleX = 430;
		TutoScreenKnuckles_F[0].BoxScaleY = 144;
		TutoScreenKnuckles_F[0].BoxX = 170;
		TutoScreenKnuckles_F[1].BoxScaleX = 455;
		TutoScreenKnuckles_F[1].BoxX = 145;
		TutoScreenKnuckles_F[4].BoxScaleX = 475;
		TutoScreenKnuckles_F[4].BoxX = 125;
		TutoScreenKnuckles_F[4].BoxScaleY = 128;
		TutoScreenKnuckles_F[5].BoxScaleY = 192;
		WriteData((__int16*)0x02BC3E9E, (__int16)2000); //Hide an extra item in Controls page
		WriteData((__int16*)0x02BC433A, (__int16)2000); //Hide an extra item in Maximum Heat page
		WriteData((__int16*)0x02BC4340, (__int16)2000); //Hide an extra item in Maximum Heat page
		//Spanish
		TutoScreenKnuckles_S[0].BoxScaleX = 370;
		TutoScreenKnuckles_S[0].BoxScaleY = 144;
		TutoScreenKnuckles_S[0].BoxX = 230;
		TutoScreenKnuckles_S[4].BoxScaleX = 415;
		TutoScreenKnuckles_S[4].BoxX = 190;
		TutoScreenKnuckles_S[4].BoxScaleY = 128;
		TutoScreenKnuckles_S[5].BoxScaleY = 192;
		//Amy
		//English
		TutoScreenAmy_E[0].BoxScaleX = 390;
		TutoScreenAmy_E[0].BoxScaleY = 144;
		TutoScreenAmy_E[0].BoxX = 210;
		TutoScreenAmy_E[4].BoxScaleY = 160;
		TutorialLayout_AmyGamma_Page1_E[1].XOffset = 136; //Rotate camera
		TutorialLayout_AmyGamma_Page1_E[1].YOffset = 0; //Rotate camera
		TutorialLayout_AmyGamma_Page1_E[2].XOffset = 136; //Maneuver character
		TutorialLayout_AmyGamma_Page1_E[2].YOffset = 24; //Maneuver character
		TutorialLayout_Amy_Page1_E[0].XOffset = 136; //A
		TutorialLayout_Amy_Page1_E[0].YOffset = 96; //A
		TutorialLayout_Amy_Page1_E[1].XOffset = 136; //B
		TutorialLayout_Amy_Page1_E[1].YOffset = 64; //B
		//Japanese
		TutoScreenAmy_J[0].BoxScaleX = 400;
		TutoScreenAmy_J[0].BoxScaleY = 144;
		TutoScreenAmy_J[0].BoxX = 200;
		TutoScreenAmy_J[4].BoxScaleY = 160;
		TutorialLayout_AmyGamma_Page1_J[1].XOffset = 136; //Rotate camera
		TutorialLayout_AmyGamma_Page1_J[1].YOffset = 0; //Rotate camera
		TutorialLayout_AmyGamma_Page1_J[2].XOffset = 136; //Maneuver character
		TutorialLayout_AmyGamma_Page1_J[2].YOffset = 24; //Maneuver character
		TutorialLayout_Amy_Page1_J[0].XOffset = 136; //A
		TutorialLayout_Amy_Page1_J[0].YOffset = 96; //A
		TutorialLayout_Amy_Page1_J[1].XOffset = 136; //B
		TutorialLayout_Amy_Page1_J[1].YOffset = 64; //B
		//German
		TutoScreenAmy_G[0].BoxScaleX = 380;
		TutoScreenAmy_G[0].BoxScaleY = 144;
		TutoScreenAmy_G[0].BoxX = 220;
		TutoScreenAmy_G[4].BoxScaleY = 160;
		WriteData((__int16*)0x02BC46FA, (__int16)2000); //Hide an extra item in controls page
		//French
		TutoScreenAmy_F[0].BoxScaleX = 425;
		TutoScreenAmy_F[0].BoxScaleY = 144;
		TutoScreenAmy_F[0].BoxX = 175;
		TutoScreenAmy_F[4].BoxScaleY = 192;
		//Spanish
		TutoScreenAmy_S[0].BoxScaleX = 370;
		TutoScreenAmy_S[0].BoxScaleY = 144;
		TutoScreenAmy_S[0].BoxX = 230;
		TutoScreenAmy_S[4].BoxScaleY = 192;
		//Big
		//English
		TutoScreenBig_E[0].BoxScaleX = 390;
		TutoScreenBig_E[0].BoxScaleY = 144;
		TutoScreenBig_E[0].BoxX = 210;
		TutoScreenBig_E[4].BoxScaleY = 128;
		TutoScreenBig_E[7].BoxScaleX = 420;
		TutoScreenBig_E[7].BoxX = 180;
		TutorialLayout_BigPage1_E[1].XOffset = 136; //Rotate camera
		TutorialLayout_BigPage1_E[1].YOffset = 0; //Rotate camera
		TutorialLayout_BigPage1_E[2].XOffset = 136; //Maneuver character
		TutorialLayout_BigPage1_E[2].YOffset = 24; //Maneuver character
		TutorialLayout_BigPage1Part2_E[0].XOffset = 136; //A
		TutorialLayout_BigPage1Part2_E[0].YOffset = 96; //A
		TutorialLayout_BigPage1Part2_E[1].XOffset = 136; //B
		TutorialLayout_BigPage1Part2_E[1].YOffset = 64; //B
		TutorialLayout_BigPage5_E[2].TexID = 32; //Hide "Tugging the rod"
		TutorialLayout_BigPage5_E[3].YOffset = 64; //Move A up
		TutorialLayout_BigPage5_E[4].YOffset = 96; //Move B up
		//Japanese
		TutoScreenBig_J[0].BoxScaleX = 400;
		TutoScreenBig_J[0].BoxScaleY = 144;
		TutoScreenBig_J[0].BoxX = 200;
		TutoScreenBig_J[4].BoxScaleY = 160;
		TutoScreenBig_J[7].BoxScaleX = 400;
		TutoScreenBig_J[7].BoxX = 200;
		TutorialLayout_BigPage1Part2_J[0].XOffset = 136; //A
		TutorialLayout_BigPage1Part2_J[0].YOffset = 96; //A
		TutorialLayout_BigPage1Part2_J[1].XOffset = 136; //B
		TutorialLayout_BigPage1Part2_J[1].YOffset = 64; //B
		//German
		TutoScreenBig_G[0].BoxScaleX = 395;
		TutoScreenBig_G[0].BoxScaleY = 144;
		TutoScreenBig_G[0].BoxX = 205;
		TutoScreenBig_G[4].BoxScaleY = 128;
		WriteData((__int16*)0x02BC4E9E, (__int16)2000); //Hide an extra item in controls page
		//French
		TutoScreenBig_F[0].BoxScaleX = 425;
		TutoScreenBig_F[0].BoxScaleY = 144;
		TutoScreenBig_F[0].BoxX = 175;
		TutoScreenBig_F[4].BoxScaleY = 128;
		TutoScreenBig_F[7].BoxScaleX = 445;
		TutoScreenBig_F[7].BoxX = 155;
		//Spanish
		TutoScreenBig_S[0].BoxScaleX = 370;
		TutoScreenBig_S[0].BoxScaleY = 144;
		TutoScreenBig_S[0].BoxX = 230;
		TutoScreenBig_S[4].BoxScaleX = 275;
		TutoScreenBig_S[4].BoxX = 325;
		TutoScreenBig_S[4].BoxScaleY = 128;
		TutoScreenBig_S[7].BoxScaleX = 420;
		TutoScreenBig_S[7].BoxX = 180;
		TutoScreenBig_S[7].BoxScaleX = 475;
		TutoScreenBig_S[7].BoxX = 125;
		//Gamma
		//English
		TutoScreenGamma_E[0].BoxScaleX = 390;
		TutoScreenGamma_E[0].BoxScaleY = 144;
		TutoScreenGamma_E[0].BoxX = 210;
		TutoScreenGamma_E[4].BoxScaleY = 160;
		TutorialLayout_Gamma_Page1_E[0].XOffset = 136; //A
		TutorialLayout_Gamma_Page1_E[0].YOffset = 96; //A
		TutorialLayout_Gamma_Page1_E[1].XOffset = 136; //B
		TutorialLayout_Gamma_Page1_E[1].YOffset = 64; //B
		//Japanese
		TutoScreenGamma_J[0].BoxScaleX = 400;
		TutoScreenGamma_J[0].BoxScaleY = 144;
		TutoScreenGamma_J[0].BoxX = 200;
		TutoScreenGamma_J[4].BoxScaleY = 160;
		TutorialLayout_Gamma_Page1_J[0].XOffset = 136; //A
		TutorialLayout_Gamma_Page1_J[0].YOffset = 96; //A
		TutorialLayout_Gamma_Page1_J[1].XOffset = 136; //B
		TutorialLayout_Gamma_Page1_J[1].YOffset = 64; //B
		//German
		TutoScreenGamma_G[4].BoxScaleY = 160;
		TutoScreenGamma_G[0].BoxScaleX = 380;
		TutoScreenGamma_G[0].BoxScaleY = 144;
		TutoScreenGamma_G[0].BoxX = 220;
		//French
		TutoScreenGamma_F[0].BoxScaleX = 425;
		TutoScreenGamma_F[0].BoxScaleY = 144;
		TutoScreenGamma_F[0].BoxX = 175;
		TutoScreenGamma_F[1].BoxScaleY = 192;
		TutoScreenGamma_F[4].BoxScaleY = 192;
		WriteData((__int16*)0x02BC4AE4, (__int16)2000); //Hide an extra item in the second page
		//Spanish
		TutoScreenGamma_S[0].BoxScaleX = 370;
		TutoScreenGamma_S[0].BoxScaleY = 144;
		TutoScreenGamma_S[0].BoxX = 230;
		TutoScreenGamma_S[4].BoxScaleY = 192;
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{
		if (GameMode != GameModes_Adventure_Story) {
			ListenToRecap = 0;
			GlobalRecapAlphaForFadeout = 0.0f;
		}
		else if (RecapScreenMode == 2 && GlobalRecapAlphaForFadeout < 1.0f) GlobalRecapAlphaForFadeout = min(1.0f, GlobalRecapAlphaForFadeout + 0.05f);
		if (TextLanguage == 3) PadManuXOffset_General = 230;
		if (TextLanguage == 4 && GetCharacterSelection() != 4) PadManuXOffset_General = 220;
		if (TextLanguage == 4 && GetCharacterSelection() == 4) PadManuXOffset_General = 205;
		if (TextLanguage != 3 && TextLanguage != 4) PadManuXOffset_General = 205;
		if (GetCharacterSelection() == 2) PadManuXOffset_F = 170; else PadManuXOffset_F = 175;
		if (GameMode == GameModes_Menu)
		{
			if (Options_ArrowScale > 0.5f) Options_ArrowScaleAmount = -0.02f;
			if (Options_ArrowScale < 0.0f) Options_ArrowScaleAmount = 0.02f;
			Options_ArrowScale = Options_ArrowScale + Options_ArrowScaleAmount;
		}
	}
}