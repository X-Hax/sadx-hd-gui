#include <SADXModLoader.h>
#include "CommonObjects.h"
#include "Objects_Shooting.h"
#include <stdlib.h>
#include <stdio.h>
#include "TutorialStuff.h"
#include "TitleScreenData.h"
#include <Trampoline.h>
#include <IniFile.hpp>

void AddGameGearTextures_all();
void ReinitScaling();
void ScaleGameGear_all();
void DrawGG_DisableFiltering(NJS_TEXTURE_VTX* a1, Int count, Uint32 gbix, Int flag);

NJS_TEXNAME TitleTexname[11];
NJS_TEXLIST TitleTexlist = {arrayptrandlength(TitleTexname)};

NJS_TEXNAME ObjectRegularTexname[100];
NJS_TEXLIST ObjectRegularTexlist = {arrayptrandlength(ObjectRegularTexname)};

void Subtitles_OnFrame();
void Subtitles_Init(const char* path, const HelperFunctions& helperFunctions);

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
static int BSInt = 0;
static float PSsX = 0;
static float PSsY = 0;
static float PSsZ = 0;
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

NJS_TEXANIM RandomRingIconPart_TEXANIM = { 32, 32, 16, 16, 0, 0, 255, 255, 95, 0x20 };

//Title screen
static float xoffset = 0;
static float sonic_xoffset = 832;
static float logo_xoffset = 800;
static bool logoback = false;

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

void DrawShittyTextures(int texnum, float x, float y, float z, float scaleX, float scaleY)
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

void DrawTitleScreenShit_cdecl(TitleScreenData* a1)
{
	NJS_COLOR StartColor = { 0x00FFFFFF };
	unsigned int v1; // ecx
	float xpos = 0;
	float ypos = 0;
	float vscale = 1.0f;
	float hzscale = 1.0f;
	float center_x = (float)HorizontalResolution / 2.0f;
	float center_y = (float)VerticalResolution / 2.0f;
	unsigned char PressStartTransparency;
	NJS_COLOR color1 = { 0xFF0000EE };
	NJS_COLOR color2 = { 0xFF0000EE };
	NJS_COLOR color3 = { 0xFF310072 };
	NJS_COLOR color4 = { 0xFF310072 };

	vscale = (float)VerticalResolution / 480.0f;
	hzscale = (float)HorizontalResolution / 640.0f;
	v1 = a1->dword30;
	if (v1 < 2) xoffset = 0;
	if (v1 >= 0x3C)
	{
		SetVtxColorA(0xFFFFFFFF);
	}
	else
	{
		SetVtxColorA(((char)(255 * v1 / 0x3C) << 24) + 0xFFFFFF);
		sonic_xoffset = ((float)HorizontalResolution / vscale) + 256.0f;
		logo_xoffset = ((float)HorizontalResolution / vscale) + 256.0f;
		logoback = false;
	}
	SetVtxColorB(0xFFFFFFFF);
	njSetTexture(&ava_title_cmn_TEXLIST);
	njTextureShadingMode(1); //Disable alpha rejection
	if (!MissedFrames)
	{
		//The full scrolling clouds image is 2560 (1280 at 1x) pixels wide
		float scale = 1280 * vscale;
		int delta = HorizontalResolution - (int)scale;
		int times = 2 + ceil(delta / scale);
		//PrintDebug("Vscale: %f, Scale: %f\n", vscale, times);
		//Draw the scrolling clouds
		ypos = -10 * vscale;
		//Draw the four rows of clouds
		xpos = 0;
		for (int q = 0; q < times + 1; q++)
		{
			for (int i = 0; i < 10; i++)
			{
				xpos = 256.0f * vscale * 0.5f * (i + 10 * q) - xoffset;
				//if (xpos <= -2560.0f/vscale) xoffset = 0;
				//PrintDebug("xpos: %f\n", xpos);
				DrawBG(27 + i, xpos, ypos, 2.0f, vscale * 0.5f, vscale * 0.5f);
				DrawBG(37 + i, xpos, ypos + 256.0f * vscale * 0.5f, 2.0f, vscale * 0.5f, vscale * 0.5f);
				DrawBG(47 + i, xpos, ypos + 512.0f * vscale * 0.5f, 2.0f, vscale * 0.5f, vscale * 0.5f);
				DrawBG(57 + i, xpos, ypos + 768.0f * vscale * 0.5f, 2.0f, vscale * 0.5f, vscale * 0.5f);
			}
		}
		if (vscale >= 0.5f) xoffset += 2 * vscale;
		else xoffset = 0;
		//PrintDebug("v1: %f\n", xoffset*vscale* 0.5f);
		//Draw the circles around Sonic (front)
		xpos = center_x - 320.0f * vscale + 280 * vscale;
		ypos = center_y - 240.0f * vscale + 52 * vscale;
		DrawBG(2, xpos, ypos, 1.4f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(3, xpos + 256.0f * vscale * 0.5f, ypos, 1.4f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(4, xpos, ypos + 256.0f * vscale * 0.5f, 1.4f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(5, xpos + 256.0f * vscale * 0.5f, ypos + 256.0f * vscale * 0.5f, 1.4f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(6, xpos, ypos + 512.0f * vscale * 0.5f, 1.4f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(7, xpos + 256.0f * vscale * 0.5f, ypos + 512.0f * vscale * 0.5f, 1.4f, vscale * 0.5f, vscale * 0.5f);
		//Draw the circles around Sonic (back)
		xpos = center_x - 320.0f * vscale - 90.0f * vscale;
		ypos = center_y - 240.0f * vscale + 52 * vscale;
		DrawBG(8, xpos, ypos, 1.3f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(9, xpos, ypos + 256.0f * vscale * 0.5f, 1.4f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(10, xpos, ypos + 512.0f * vscale * 0.5f, 1.4f, vscale * 0.5f, vscale * 0.5f);
		//Draw the gradient overlay/border thing
		//Left
		if (v1 <= 60)
		{
			color1.argb.a = int(((float)v1 / 60.0f) * 255);
			color2.argb.a = int(((float)v1 / 60.0f) * 255);
			color3.argb.a = int(((float)v1 / 60.0f) * 255);
			color4.argb.a = int(((float)v1 / 60.0f) * 255);
		}
		else
		{
			color1.argb.a = 255;
			color2.argb.a = 255;
			color3.argb.a = 255;
			color4.argb.a = 255;
		}
		ScreenTextureVertices[0].col = color1.color;
		ScreenTextureVertices[1].col = color2.color;
		//Right
		ScreenTextureVertices[2].col = color3.color;
		ScreenTextureVertices[3].col = color4.color;
		DrawBG(1, 0, 0, 1.3f, (float)HorizontalResolution / 256.0f, vscale * 0.5f);
		ypos = (float)VerticalResolution - 256.0f * vscale * 0.5f;
		DrawBG(0x40000001, 0, ypos, 1.3f, (float)HorizontalResolution / 256.0f, vscale * 0.5f);
		SetVtxColorB(0xFFFFFFFF);
		//Draw copyright text
		xpos = center_x - 40.0f * vscale;
		ypos = center_y + 140.0f * vscale;
		DrawBG(11, xpos, ypos, 1.3f, vscale * 0.4f, vscale * 0.4f);
		DrawBG(12, xpos + 256.0f * vscale * 0.4f, ypos, 1.3f, vscale * 0.4f, vscale * 0.4f);
		DrawBG(13, xpos + 512.0f * vscale * 0.4f, ypos, 1.3f, vscale * 0.4f, vscale * 0.4f);
		//Draw the lines above and below the gradients
		DrawBG(0, 0, 0, 1.3f, (float)HorizontalResolution / 256.0f, vscale * 0.5f);
		ypos = (float)VerticalResolution - 256.0f * vscale * 0.5f;
		DrawBG(0x40000000, 0, ypos, 1.3f, (float)HorizontalResolution / 256.0f, vscale * 0.5f);
		//Draw Sonic
		xpos = center_x - 320.0f * vscale - (128.0f - sonic_xoffset) * vscale;
		ypos = center_y - 260.0f * vscale;
		if (v1 >= 60)
		{
			if (sonic_xoffset > 63) sonic_xoffset -= 64; else sonic_xoffset = 0;
		}
		DrawBG(14, xpos, ypos, 1.2f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(15, xpos + 256.0f * vscale * 0.5f, ypos, 1.2f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(16, xpos + 512.0f * vscale * 0.5f, ypos, 1.2f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(17, xpos + 256.0f * vscale * 0.5f, ypos + 256.0f * vscale * 0.5f, 1.2f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(18, xpos + 512.0f * vscale * 0.5f, ypos + 256.0f * vscale * 0.5f, 1.2f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(19, xpos, ypos + 512.0f * vscale * 0.5f, 1.2f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(20, xpos + 256.0f * vscale * 0.5f, ypos + 512.0f * vscale * 0.5f, 1.2f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(21, xpos + 512.0f * vscale * 0.5f, ypos + 512.0f * vscale * 0.5f, 1.2f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(22, xpos + 768.0f * vscale * 0.5f, ypos + 512.0f * vscale * 0.5f, 1.2f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(23, xpos, ypos + 768.0f * vscale * 0.5f, 1.2f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(24, xpos + 256.0f * vscale * 0.5f, ypos + 768.0f * vscale * 0.5f, 1.2f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(25, xpos + 512.0f * vscale * 0.5f, ypos + 768.0f * vscale * 0.5f, 1.2f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(26, xpos + 768.0f * vscale * 0.5f, ypos + 768.0f * vscale * 0.5f, 1.2f, vscale * 0.5f, vscale * 0.5f);
		//Draw logo
		njSetTexture(&ava_gtitle0_e_TEXLIST);
		xpos = center_x - (171.0f + logo_xoffset) * vscale;
		ypos = center_y - 143.0f * vscale;
		DrawBG(0, xpos, ypos, 1.1f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(1, xpos + 256.0f * vscale * 0.5f, ypos, 1.1f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(2, xpos + 512.0f * vscale * 0.5f, ypos, 1.1f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(3, xpos + 768.0f * vscale * 0.5f, ypos, 1.1f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(4, xpos, ypos + 256.0f * vscale * 0.5f, 1.1f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(5, xpos + 256.0f * vscale * 0.5f, ypos + 256.0f * vscale * 0.5f, 1.1f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(6, xpos + 512.0f * vscale * 0.5f, ypos + 256.0f * vscale * 0.5f, 1.1f, vscale * 0.5f, vscale * 0.5f);
		DrawBG(7, xpos + 768.0f * vscale * 0.5f, ypos + 256.0f * vscale * 0.5f, 1.1f, vscale * 0.5f, vscale * 0.5f);
		//PrintDebug("logo: %f\n", logo_xoffset);
		//PrintDebug("Start: %d\n", a1->dword28);
		if (v1 >= 80)
		{
			if (!logoback)
			{
				if (logo_xoffset > -128) logo_xoffset -= 64;
				else
				{
					logoback = true;
				}
			}
			else
			{
				if (logo_xoffset < -63) logo_xoffset += 64; else logo_xoffset = 0;
			}
		}
		//Increase timer
		a1->dword30 += MissedFrames_B;
		//Draw Press Start
		if (a1->dword24 >= 150)
		{
			PressStartTransparency = a1->dword28;
			if ((a1->dword28 & 0x1FFu) >= 0x100)
			{
				PressStartTransparency = -1 - a1->dword28;
			}
			StartColor.argb.a = PressStartTransparency;
			SetVtxColorB(StartColor.color);
			xpos = center_x + 32.0f * vscale;
			ypos = center_y + 84 * vscale;
			njTextureShadingMode(1);
			njSetTexture(&ava_gtitle0_e_TEXLIST);
			DrawBG(8, xpos, ypos, 1.1f, vscale * 0.5f, vscale * 0.5f);
			DrawBG(9, xpos + 128.0f * vscale, ypos, 1.1f, vscale * 0.5f, vscale * 0.5f);
			njTextureShadingMode(2); 
			SetVtxColorB(0xFFFFFFFF);
		}
	}
}

static void __declspec(naked) DrawTitleScreenShit_asm()
{
	__asm
	{
		push esi // a1

		// Call your __cdecl function here:
		call DrawTitleScreenShit_cdecl

		pop esi // a1
		retn
	}
}

void DrawChnamBShit(Uint8 index)
{
	Direct3D_SetZFunc(index);
	Direct3D_EnableZWrite(1);
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
		//Title screen
		WriteCall((void*)0x51037E, DrawTitleScreenShit_asm);
		//Game Gear scaling
		/*
		ReinitScaling();
		AddGameGearTextures_all();
		ScaleGameGear_all();
		WriteCall((void*)0x6FE9F8, DrawGG_DisableFiltering);
		*/
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
		//Subtitle system
		Subtitles_Init(path, helperFunctions);
		//Texture replacements
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
		ReplacePVMX("AVA_TITLE_CMN");
		ReplacePVMX("AVA_STNAM_E");
		ReplacePVMX("AVA_SUUJI");
		if (ava_title_back_e_TEXLIST.nbTexture == 8) //Check if DC Conversion replaces TITLE_BACK and GTITLE0
		{
			//ReplacePVMX("AVA_TITLE_BACK"); - JP title screen, not used yet
			ReplacePVMX("AVA_TITLE_BACK_E"); 
			ReplacePVMX("AVA_GTITLE0_E");
		}
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
		WriteData<5>((void*)0x0040BE0D, 0x90); //Disable "Now loading..."
		WriteData<5>((void*)0x00503438, 0x90); //Disable "Now loading..."
		WriteData<5>((void*)0x0050346D, 0x90); //Disable "Now loading..."
		//Character select screen fixes
		WriteCall((void*)0x00511AD0, RetrievePlayerSelectStuff); //Player select text in character select screen
		WriteCall((void*)0x00511C76, RetrieveBottomThingStuff); //Bottom thing in character select screen
		WriteCall((void*)0x00511B3B, DrawShittyTextures); //Draw stuff that refuses to render properly otherwise
		WriteCall((void*)0x00511E47, DrawChnamBShit); //Fix disappearing character name after loading a different save
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
		OBJ_REGULAR_TEXLIST = ObjectRegularTexlist; //Added DC ripple texture
		ava_title_e_TEXLIST = TitleTexlist; //Added Internet option
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
		TutoScreenXSonic_E[0].BoxScaleX = 390;
		TutoScreenXSonic_E[0].BoxScaleY = 144;
		TutoScreenXSonic_E[0].BoxX = 210;
		TutoScreenXSonic_E[4].BoxScaleY = 160;
		TutorialLayoutX_Sonic_Page1_E[1].XOffset = 136; //Rotate camera
		TutorialLayoutX_Sonic_Page1_E[1].YOffset = 0; //Rotate camera
		TutorialLayoutX_Sonic_Page1_E[2].XOffset = 136; //Maneuver character
		TutorialLayoutX_Sonic_Page1_E[2].YOffset = 24; //Maneuver character
		TutorialLayoutX_Sonic_Page1_E[4].XOffset = 136; //A button
		TutorialLayoutX_Sonic_Page1_E[4].YOffset = 64; //A button
		TutorialLayoutX_Sonic_Page1_E[4].TexID = 3;
		TutorialLayoutX_Sonic_Page1_E[5].TexID = 4;
		TutorialLayoutX_Sonic_Page1_E[5].XOffset = 136;
		TutorialLayoutX_Sonic_Page1_E[5].YOffset = 96;
		WriteData((__int16*)0x02BC3AE2, (__int16)2000); //Hide an extra item in controls page (all langs)
		//Japanese
		TutoScreenXSonic_J[0].BoxScaleX = 400;
		TutoScreenXSonic_J[0].BoxScaleY = 144;
		TutoScreenXSonic_J[0].BoxX = 200;
		TutoScreenXSonic_J[4].BoxScaleY = 160;
		TutorialLayoutX_Sonic_Page1_J[1].XOffset = 136; //Rotate camera
		TutorialLayoutX_Sonic_Page1_J[1].YOffset = 0; //Rotate camera
		TutorialLayoutX_Sonic_Page1_J[2].XOffset = 136; //Maneuver character
		TutorialLayoutX_Sonic_Page1_J[2].YOffset = 24; //Maneuver character
		TutorialLayoutX_Sonic_Page1_J[3].XOffset = 136; //A button
		TutorialLayoutX_Sonic_Page1_J[3].YOffset = 64; //A button
		TutorialLayoutX_Sonic_Page1_J[3].TexID = 3;
		TutorialLayoutX_Sonic_Page1_J[4].TexID = 4;
		TutorialLayoutX_Sonic_Page1_J[4].XOffset = 136;
		TutorialLayoutX_Sonic_Page1_J[4].YOffset = 96;
		//German
		TutoScreenXSonic_G[0].BoxScaleX = 380;
		TutoScreenXSonic_G[0].BoxScaleY = 144;
		TutoScreenXSonic_G[0].BoxX = 220;
		TutoScreenXSonic_G[3].BoxScaleY = 160;
		TutoScreenXSonic_G[4].BoxScaleY = 160;
		//French
		TutoScreenXSonic_F[4].BoxScaleY = 192;
		TutoScreenXSonic_F[0].BoxScaleX = 425;
		TutoScreenXSonic_F[0].BoxScaleY = 144;
		TutoScreenXSonic_F[0].BoxX = 175;
		//Spanish
		TutoScreenXSonic_S[0].BoxScaleX = 370;
		TutoScreenXSonic_S[0].BoxScaleY = 144;
		TutoScreenXSonic_S[0].BoxX = 230;
		TutoScreenXSonic_S[4].BoxScaleY = 192;
		//Tails
		//English
		TutoScreenXTails_E[0].BoxScaleX = 390;
		TutoScreenXTails_E[0].BoxScaleY = 144;
		TutoScreenXTails_E[0].BoxX = 210;
		TutoScreenXTails_E[4].BoxScaleY = 160;
		TutorialLayoutX_SharedTailsKnucklesPage1_E[0].XOffset = 136; //Rotate camera
		TutorialLayoutX_SharedTailsKnucklesPage1_E[0].YOffset = 0; //Rotate camera
		TutorialLayoutX_SharedTailsKnucklesPage1_E[1].XOffset = 136; //Maneuver character
		TutorialLayoutX_SharedTailsKnucklesPage1_E[1].YOffset = 24; //Maneuver character
		TutorialLayoutX_SharedTailsKnucklesPage1_E[3].XOffset = 136; //A button
		TutorialLayoutX_SharedTailsKnucklesPage1_E[3].YOffset = 96; //A button
		TutorialLayoutX_Tails_Page1_E.XOffset = 136; //Tail attack
		TutorialLayoutX_Tails_Page1_E.YOffset = 64; //Tail attack
		//Japanese
		TutoScreenXTails_J[0].BoxScaleX = 400;
		TutoScreenXTails_J[0].BoxScaleY = 144;
		TutoScreenXTails_J[0].BoxX = 200;
		TutoScreenXTails_J[4].BoxScaleY = 160;
		TutorialLayoutX_SharedTailsKnucklesPage1_J[0].XOffset = 136; //Rotate camera
		TutorialLayoutX_SharedTailsKnucklesPage1_J[0].YOffset = 0; //Rotate camera
		TutorialLayoutX_SharedTailsKnucklesPage1_J[1].XOffset = 136; //Maneuver character
		TutorialLayoutX_SharedTailsKnucklesPage1_J[1].YOffset = 24; //Maneuver character
		TutorialLayoutX_SharedTailsKnucklesPage1_J[2].XOffset = 136; //A button
		TutorialLayoutX_SharedTailsKnucklesPage1_J[2].YOffset = 96; //A button
		TutorialLayoutX_Tails_Page1_J.XOffset = 136; //Tail attack
		TutorialLayoutX_Tails_Page1_J.YOffset = 64; //Tail attack
		//French
		TutoScreenXTails_F[4].BoxScaleY = 192;
		TutoScreenXTails_F[0].BoxScaleX = 425;
		TutoScreenXTails_F[0].BoxScaleY = 144;
		TutoScreenXTails_F[0].BoxX = 175;
		//German
		TutoScreenXTails_G[0].BoxScaleX = 390;
		TutoScreenXTails_G[0].BoxScaleY = 144;
		TutoScreenXTails_G[0].BoxX = 210;
		TutoScreenXTails_G[3].BoxScaleY = 128;
		TutoScreenXTails_G[4].BoxScaleY = 160;
		TutoScreenXTails_G[0].BoxScaleX = 380;
		TutoScreenXTails_G[0].BoxScaleY = 144;
		TutoScreenXTails_G[0].BoxX = 220;
		WriteData((__int16*)0x02BC3E9E, (__int16)2000); //Hide an extra item in controls page
		//Spanish
		TutoScreenXTails_S[0].BoxScaleX = 370;
		TutoScreenXTails_S[0].BoxScaleY = 144;
		TutoScreenXTails_S[0].BoxX = 230;
		TutoScreenXTails_S[4].BoxScaleY = 192;
		//Knuckles
		//English
		TutoScreenXKnuckles_E[0].BoxScaleX = 390;
		TutoScreenXKnuckles_E[0].BoxScaleY = 144;
		TutoScreenXKnuckles_E[0].BoxX = 210;
		TutoScreenXKnuckles_E[4].BoxX = 180;
		TutoScreenXKnuckles_E[4].BoxScaleY = 128;
		TutoScreenXKnuckles_E[4].BoxScaleX = 425;
		TutoScreenXKnuckles_E[5].BoxScaleY = 160;
		TutorialLayoutX_Knuckles_Page1_E.XOffset = 136; //Punch attack
		TutorialLayoutX_Knuckles_Page1_E.YOffset = 64; //Punch attack
		WriteData<1>((char*)0x02BC4308, 0x03); //Number of items in Maximum Heat screen, remove unnecessary line
		//Japanese
		TutoScreenXKnuckles_J[0].BoxScaleX = 400;
		TutoScreenXKnuckles_J[0].BoxScaleY = 144;
		TutoScreenXKnuckles_J[0].BoxX = 200;
		TutoScreenXKnuckles_J[4].BoxX = 180;
		TutoScreenXKnuckles_J[4].BoxScaleY = 128;
		TutoScreenXKnuckles_J[4].BoxScaleX = 425;
		TutoScreenXKnuckles_J[5].BoxScaleY = 160;
		TutorialLayoutX_Knuckles_Page1_J.XOffset = 136; //Punch attack
		TutorialLayoutX_Knuckles_Page1_J.YOffset = 64; //Punch attack
		//German
		TutoScreenXKnuckles_G[0].BoxScaleX = 380;
		TutoScreenXKnuckles_G[0].BoxScaleY = 144;
		TutoScreenXKnuckles_G[0].BoxX = 220;
		TutoScreenXKnuckles_G[2].BoxScaleY = 128;
		TutoScreenXKnuckles_G[4].BoxX = 180;
		TutoScreenXKnuckles_G[4].BoxScaleX = 420;
		TutoScreenXKnuckles_G[4].BoxScaleY = 128;
		TutoScreenXKnuckles_G[5].BoxScaleY = 160;
		WriteData((__int16*)0x02BC42E0, (__int16)2000); //Hide an extra item in Climbing page
		//French
		TutoScreenXKnuckles_F[0].BoxScaleX = 430;
		TutoScreenXKnuckles_F[0].BoxScaleY = 144;
		TutoScreenXKnuckles_F[0].BoxX = 170;
		TutoScreenXKnuckles_F[1].BoxScaleX = 455;
		TutoScreenXKnuckles_F[1].BoxX = 145;
		TutoScreenXKnuckles_F[4].BoxScaleX = 475;
		TutoScreenXKnuckles_F[4].BoxX = 125;
		TutoScreenXKnuckles_F[4].BoxScaleY = 128;
		TutoScreenXKnuckles_F[5].BoxScaleY = 192;
		WriteData((__int16*)0x02BC3E9E, (__int16)2000); //Hide an extra item in Controls page
		WriteData((__int16*)0x02BC433A, (__int16)2000); //Hide an extra item in Maximum Heat page
		WriteData((__int16*)0x02BC4340, (__int16)2000); //Hide an extra item in Maximum Heat page
		//Spanish
		TutoScreenXKnuckles_S[0].BoxScaleX = 370;
		TutoScreenXKnuckles_S[0].BoxScaleY = 144;
		TutoScreenXKnuckles_S[0].BoxX = 230;
		TutoScreenXKnuckles_S[4].BoxScaleX = 415;
		TutoScreenXKnuckles_S[4].BoxX = 190;
		TutoScreenXKnuckles_S[4].BoxScaleY = 128;
		TutoScreenXKnuckles_S[5].BoxScaleY = 192;
		//Amy
		//English
		TutoScreenXAmy_E[0].BoxScaleX = 390;
		TutoScreenXAmy_E[0].BoxScaleY = 144;
		TutoScreenXAmy_E[0].BoxX = 210;
		TutoScreenXAmy_E[4].BoxScaleY = 160;
		TutorialLayoutX_AmyGamma_Page1_E[1].XOffset = 136; //Rotate camera
		TutorialLayoutX_AmyGamma_Page1_E[1].YOffset = 0; //Rotate camera
		TutorialLayoutX_AmyGamma_Page1_E[2].XOffset = 136; //Maneuver character
		TutorialLayoutX_AmyGamma_Page1_E[2].YOffset = 24; //Maneuver character
		TutorialLayoutX_Amy_Page1_E[0].XOffset = 136; //A
		TutorialLayoutX_Amy_Page1_E[0].YOffset = 96; //A
		TutorialLayoutX_Amy_Page1_E[1].XOffset = 136; //B
		TutorialLayoutX_Amy_Page1_E[1].YOffset = 64; //B
		//Japanese
		TutoScreenXAmy_J[0].BoxScaleX = 400;
		TutoScreenXAmy_J[0].BoxScaleY = 144;
		TutoScreenXAmy_J[0].BoxX = 200;
		TutoScreenXAmy_J[4].BoxScaleY = 160;
		TutorialLayoutX_AmyGamma_Page1_J[1].XOffset = 136; //Rotate camera
		TutorialLayoutX_AmyGamma_Page1_J[1].YOffset = 0; //Rotate camera
		TutorialLayoutX_AmyGamma_Page1_J[2].XOffset = 136; //Maneuver character
		TutorialLayoutX_AmyGamma_Page1_J[2].YOffset = 24; //Maneuver character
		TutorialLayoutX_Amy_Page1_J[0].XOffset = 136; //A
		TutorialLayoutX_Amy_Page1_J[0].YOffset = 96; //A
		TutorialLayoutX_Amy_Page1_J[1].XOffset = 136; //B
		TutorialLayoutX_Amy_Page1_J[1].YOffset = 64; //B
		//German
		TutoScreenXAmy_G[0].BoxScaleX = 380;
		TutoScreenXAmy_G[0].BoxScaleY = 144;
		TutoScreenXAmy_G[0].BoxX = 220;
		TutoScreenXAmy_G[4].BoxScaleY = 160;
		WriteData((__int16*)0x02BC46FA, (__int16)2000); //Hide an extra item in controls page
		//French
		TutoScreenXAmy_F[0].BoxScaleX = 425;
		TutoScreenXAmy_F[0].BoxScaleY = 144;
		TutoScreenXAmy_F[0].BoxX = 175;
		TutoScreenXAmy_F[4].BoxScaleY = 192;
		//Spanish
		TutoScreenXAmy_S[0].BoxScaleX = 370;
		TutoScreenXAmy_S[0].BoxScaleY = 144;
		TutoScreenXAmy_S[0].BoxX = 230;
		TutoScreenXAmy_S[4].BoxScaleY = 192;
		//Big
		//English
		TutoScreenXBig_E[0].BoxScaleX = 390;
		TutoScreenXBig_E[0].BoxScaleY = 144;
		TutoScreenXBig_E[0].BoxX = 210;
		TutoScreenXBig_E[4].BoxScaleY = 128;
		TutoScreenXBig_E[7].BoxScaleX = 420;
		TutoScreenXBig_E[7].BoxX = 180;
		TutorialLayoutX_BigPage1_E[1].XOffset = 136; //Rotate camera
		TutorialLayoutX_BigPage1_E[1].YOffset = 0; //Rotate camera
		TutorialLayoutX_BigPage1_E[2].XOffset = 136; //Maneuver character
		TutorialLayoutX_BigPage1_E[2].YOffset = 24; //Maneuver character
		TutorialLayoutX_BigPage1Part2_E[0].XOffset = 136; //A
		TutorialLayoutX_BigPage1Part2_E[0].YOffset = 96; //A
		TutorialLayoutX_BigPage1Part2_E[1].XOffset = 136; //B
		TutorialLayoutX_BigPage1Part2_E[1].YOffset = 64; //B
		TutorialLayoutX_BigPage5_E[2].TexID = 32; //Hide "Tugging the rod"
		TutorialLayoutX_BigPage5_E[3].YOffset = 64; //Move A up
		TutorialLayoutX_BigPage5_E[4].YOffset = 96; //Move B up
		//Japanese
		TutoScreenXBig_J[0].BoxScaleX = 400;
		TutoScreenXBig_J[0].BoxScaleY = 144;
		TutoScreenXBig_J[0].BoxX = 200;
		TutoScreenXBig_J[4].BoxScaleY = 160;
		TutoScreenXBig_J[7].BoxScaleX = 400;
		TutoScreenXBig_J[7].BoxX = 200;
		TutorialLayoutX_BigPage1Part2_J[0].XOffset = 136; //A
		TutorialLayoutX_BigPage1Part2_J[0].YOffset = 96; //A
		TutorialLayoutX_BigPage1Part2_J[1].XOffset = 136; //B
		TutorialLayoutX_BigPage1Part2_J[1].YOffset = 64; //B
		//German
		TutoScreenXBig_G[0].BoxScaleX = 395;
		TutoScreenXBig_G[0].BoxScaleY = 144;
		TutoScreenXBig_G[0].BoxX = 205;
		TutoScreenXBig_G[4].BoxScaleY = 128;
		WriteData((__int16*)0x02BC4E9E, (__int16)2000); //Hide an extra item in controls page
		//French
		TutoScreenXBig_F[0].BoxScaleX = 425;
		TutoScreenXBig_F[0].BoxScaleY = 144;
		TutoScreenXBig_F[0].BoxX = 175;
		TutoScreenXBig_F[4].BoxScaleY = 128;
		TutoScreenXBig_F[7].BoxScaleX = 445;
		TutoScreenXBig_F[7].BoxX = 155;
		//Spanish
		TutoScreenXBig_S[0].BoxScaleX = 370;
		TutoScreenXBig_S[0].BoxScaleY = 144;
		TutoScreenXBig_S[0].BoxX = 230;
		TutoScreenXBig_S[4].BoxScaleX = 275;
		TutoScreenXBig_S[4].BoxX = 325;
		TutoScreenXBig_S[4].BoxScaleY = 128;
		TutoScreenXBig_S[7].BoxScaleX = 420;
		TutoScreenXBig_S[7].BoxX = 180;
		TutoScreenXBig_S[7].BoxScaleX = 475;
		TutoScreenXBig_S[7].BoxX = 125;
		//Gamma
		//English
		TutoScreenXGamma_E[0].BoxScaleX = 390;
		TutoScreenXGamma_E[0].BoxScaleY = 144;
		TutoScreenXGamma_E[0].BoxX = 210;
		TutoScreenXGamma_E[4].BoxScaleY = 160;
		TutorialLayoutX_Gamma_Page1_E[0].XOffset = 136; //A
		TutorialLayoutX_Gamma_Page1_E[0].YOffset = 96; //A
		TutorialLayoutX_Gamma_Page1_E[1].XOffset = 136; //B
		TutorialLayoutX_Gamma_Page1_E[1].YOffset = 64; //B
		//Japanese
		TutoScreenXGamma_J[0].BoxScaleX = 400;
		TutoScreenXGamma_J[0].BoxScaleY = 144;
		TutoScreenXGamma_J[0].BoxX = 200;
		TutoScreenXGamma_J[4].BoxScaleY = 160;
		TutorialLayoutX_Gamma_Page1_J[0].XOffset = 136; //A
		TutorialLayoutX_Gamma_Page1_J[0].YOffset = 96; //A
		TutorialLayoutX_Gamma_Page1_J[1].XOffset = 136; //B
		TutorialLayoutX_Gamma_Page1_J[1].YOffset = 64; //B
		//German
		TutoScreenXGamma_G[4].BoxScaleY = 160;
		TutoScreenXGamma_G[0].BoxScaleX = 380;
		TutoScreenXGamma_G[0].BoxScaleY = 144;
		TutoScreenXGamma_G[0].BoxX = 220;
		//French
		TutoScreenXGamma_F[0].BoxScaleX = 425;
		TutoScreenXGamma_F[0].BoxScaleY = 144;
		TutoScreenXGamma_F[0].BoxX = 175;
		TutoScreenXGamma_F[1].BoxScaleY = 192;
		TutoScreenXGamma_F[4].BoxScaleY = 192;
		WriteData((__int16*)0x02BC4AE4, (__int16)2000); //Hide an extra item in the second page
		//Spanish
		TutoScreenXGamma_S[0].BoxScaleX = 370;
		TutoScreenXGamma_S[0].BoxScaleY = 144;
		TutoScreenXGamma_S[0].BoxX = 230;
		TutoScreenXGamma_S[4].BoxScaleY = 192;
	}
	__declspec(dllexport) void __cdecl OnFrame()
	{
		Subtitles_OnFrame();
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
	__declspec(dllexport) void __cdecl OnRenderDeviceReset()
	{
		/*
		ReinitScaling();
		ScaleGameGear_all();
		*/
		xoffset = 0;
	}
}