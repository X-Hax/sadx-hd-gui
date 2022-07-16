#include <SADXModLoader.h>
#include <stdlib.h>
#include <stdio.h>
#include <Trampoline.h>
#include <IniFile.hpp>
#include "Common.h"

NJS_TEXNAME textures_obj_regular[100];
NJS_TEXLIST texlist_obj_regular = { arrayptrandlength(textures_obj_regular) };

NJS_TEXNAME ava_title_e_hd_textures[11];
NJS_TEXLIST ava_title_e_hd_texlist = { arrayptrandlength(ava_title_e_hd_textures) };

NJS_TEXNAME ava_title_cmn_hd_textures[70];
NJS_TEXLIST ava_title_cmn_hd_texlist = { arrayptrandlength(ava_title_cmn_hd_textures) };

void Subtitles_OnFrame();
void Subtitles_Init(const char* path, const HelperFunctions& helperFunctions);

void Mission_Init(const char* path, const HelperFunctions& helperFunctions);

void TexturesDC_Init(const char* path, const HelperFunctions& helperFunctions);

void Tutorials_Init(const char* path, const HelperFunctions& helperFunctions);
void Tutorials_OnFrame();

void TitleScreen_Init();
void TitleScreen_OnReset();

void GameGear_Init();
void GenerateGameGearBackground();

#define ReplacePNG_Common(a) do { \
	_snprintf_s(pathbuf, LengthOfArray(pathbuf), "%s\\textures\\pvr_common\\index.txt", path); \
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
		// Warnings
		if (helperFunctions.Version < 6)
		{
			MessageBox(WindowHandle,
				L"Mod Loader out of date. HD GUI requires API version 6 or newer.",
				L"HD GUI error: Mod loader out of date", MB_OK | MB_ICONERROR);
			return;
		}
		TitleScreen_Init();
		GameGear_Init();
		Subtitles_Init(path, helperFunctions);
		Tutorials_Init(path, helperFunctions);
		Mission_Init(path, helperFunctions);
		if (GetModuleHandle(L"DCMods_Main") != nullptr)
		{
			TexturesDC_Init(path, helperFunctions); // Texture replacements for DC Conversion
			// Temporary hack until the new update is released
			helperFunctions.ReplaceFile("system\\OBJ_REGULAR.PVM", "system\\OBJ_REGULAR_DC.PVM"); 
			OBJ_REGULAR_TEXLIST = texlist_obj_regular;
		}
		// Fix random ring icon
		WriteData((NJS_TEXANIM**)0x004C03FF, &RandomRingIconPart_TEXANIM);
		// Various fixes
		WriteCall((void*)0x00457F2F, DrawSprite_Hook);
		WriteCall((void*)0x00504DC4, HelpAvaSquareThing);
		WriteCall((void*)0x0050717E, ScaleCharselJapaneseText_LikeSeriouslyWTF);
		WriteCall((void*)0x005070E2, ScaleCharselJapaneseText_LikeSeriouslyWTF);
		WriteData<1>((char*)0x0091C00C, 0i8); // You win/lose text alignment
		WriteData<1>((char*)0x0091C00E, 0i8); // You win/lose text alignment
		WriteData<1>((char*)0x0091C020, 0i8); // You win/lose text alignment
		WriteData<1>((char*)0x0091C022, 0i8); // You win/lose text alignment
		WriteData<1>((char*)0x0091C034, 0i8); // You win/lose text alignment
		WriteData<1>((char*)0x0091C048, 0i8); // You win/lose text alignment
		// Texture replacements
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
		if (ava_title_back_e_TEXLIST.nbTexture == 8) // Check if DC Conversion replaces TITLE_BACK and GTITLE0
		{
			//ReplacePVMX("AVA_TITLE_BACK"); - JP title screen, not used yet
			ReplacePVMX("AVA_TITLE_BACK_E"); 
			ReplacePVMX("AVA_GTITLE0_E");
		}
		ReplacePVMX("AVA_TITLE");
		ReplacePVMX("AVA_TITLE_E");
		ReplacePVMX("AVA_TRIALACTSEL");
		ReplacePVMX("AVA_TRIALACTSEL_E");
		ReplacePVMX("BOARD_SCORE");
		ReplacePVMX("B_CHNAM");
		ReplacePVMX("B_CHNAM_E");
		ReplacePVMX("CHAOS_LIFEGAUGE");
		ReplacePVMX("CON_REGULAR");
		ReplacePVMX("CON_REGULAR_E");
		ReplacePVMX("E102TIME");
		ReplacePVMX("EXTRA");
		ReplacePVMX("ENDBG_LOGO");
		ReplacePVMX("GAMEOVER");
		ReplacePVMX("GAMEOVER_E");
		ReplacePVMX("GG_TEXLIST_US");
		ReplacePVMX("GGMENU_TEXLIST_US");
		ReplacePVMX("AL_DX_TEX_XYBUTTON");
		ReplacePVMX("AL_STG_KINDER_AD_TEX");
		ReplacePVMX("AL_TEX_COMMON");
		ReplacePVMX("AL_TEX_ODEKAKE_MENU_EN");
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
		ReplacePVMX("SMRYBG_AMY");
		ReplacePVMX("SMRYBG_BIG");
		ReplacePVMX("SMRYBG_E102");
		ReplacePVMX("SMRYBG_KNUCKLES");
		ReplacePVMX("SMRYBG_SONIC");
		ReplacePVMX("SMRYBG_SUPERSONIC");
		ReplacePVMX("SMRYBG_TAILS");
		ReplacePVMX("TX_CHNAM");
		ReplacePVMX("TX_CHNAM_E");
		// Common PVRs
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
		// English stage PVRs
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
		// Japanese stage PVRs
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
		// Screen fade fixes
		f480_Fixed = 1.0f + VerticalResolution;
		f640_Fixed = 1.0f + HorizontalResolution;
		WriteData((float**)0x00433385, &f480_Fixed); // Screen fade resolution
		WriteData((float**)0x004333A6, &f640_Fixed); // Screen fade resolution
		WriteCall((void*)0x0042BF52, ScreenFadeFix);
		// Character select screen fixes
		WriteCall((void*)0x00511AD0, RetrievePlayerSelectStuff); // Player select text in character select screen
		WriteCall((void*)0x00511C76, RetrieveBottomThingStuff); // Bottom thing in character select screen
		WriteCall((void*)0x00511B3B, DrawShittyTextures); // Draw stuff that refuses to render properly otherwise
		WriteCall((void*)0x00511E47, DrawChnamBShit); // Fix disappearing character name after loading a different save
		WriteCall((void*)0x00511A8B, DisplayScreenTexture_AlwaysTop); // Move the "Select your character" text to top
		WriteData<5>((void*)0x00511C18, 0x90); // Disable ZFunc stuff to prevent character model overlap issues
		// Shadow blending fixes
		WriteCall((void*)0x0050B584, DrawShadow_Hook);
		WriteCall((void*)0x00431D37, DrawShadow_Hook);
		WriteCall((void*)0x00506EFF, DrawShadow_Hook);
		WriteCall((void*)0x0050D8B3, DrawShadow_Hook);
		WriteCall((void*)0x0050B61A, DrawShadow_Hook); // Main menu (trial) shadow
		WriteCall((void*)0x00508FFD, DrawTexture_Hook); // Sound test icon
		WriteCall((void*)0x00509130, DrawTexture_Hook); // Sonic icon background
		WriteCall((void*)0x00509191, DrawTexture_Hook); // Sonic icon
		WriteCall((void*)0x00509439, DrawTexture_Hook); // Languages icon
		WriteCall((void*)0x0050952F, DrawTexture_Hook); // Rumble icon
		WriteCall((void*)0x0050782A, DrawTexture_Hook); // AVA_SAN triangle shadow
		// Fixes for the question mark in the Character Select menu
		((NJS_OBJECT*)0x010D7774)->basicdxmodel->mats[0].diffuse.color = 0xFFB2B2B2;
		((NJS_OBJECT*)0x010D7774)->basicdxmodel->mats[0].attr_texId = 10;
		ava_title_e_TEXLIST = ava_title_e_hd_texlist; // Added Internet option
		ava_title_cmn_TEXLIST = ava_title_cmn_hd_texlist;
		WriteCall((void*)0x005092A1, FileIcon_Hook); // File icon
	}

	__declspec(dllexport) void __cdecl OnFrame()
	{
		Subtitles_OnFrame();
		Tutorials_OnFrame();

		if (GameMode == GameModes_Menu)
		{
			if (Options_ArrowScale > 0.5f) 
				Options_ArrowScaleAmount = -0.02f;
			if (Options_ArrowScale < 0.0f) 
				Options_ArrowScaleAmount = 0.02f;
			Options_ArrowScale = Options_ArrowScale + Options_ArrowScaleAmount;
		}
	}

	__declspec(dllexport) void __cdecl OnRenderDeviceReset()
	{
		TitleScreen_OnReset();
		GenerateGameGearBackground();
	}
}