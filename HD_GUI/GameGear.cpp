#include <SADXModLoader.h>
#include "GameGear.h"

AdvertiseBaseParam GameGearBackgroundParams[64];

DataPointer(NJS_TEXLIST, GG_TEXLIST_TEXLIST, 0x339CC44);

static float HorizontalResolution_Float = 640.0f;

// Recreates background textures for the Mini Game Collection in widescreen.
void GenerateGameGearBackground()
{
	HorizontalResolution_Float = (float)HorizontalResolution;
	float startpoint = HorizontalResolution_Float;
	int index = 0;
	for (int b = 0; b < 8; b++)
	{
		// Top/bottom
		for (int i = 0; i < 2; i++)
		{
			// Top
			GameGearBackgroundParams[index].Color = 0xFFFFFFFF;
			GameGearBackgroundParams[index].Priority = 0.3f;
			GameGearBackgroundParams[index].Size.x = 256.0f;
			GameGearBackgroundParams[index].Size.y = 256.0f;
			GameGearBackgroundParams[index].TexList = &GG_TEXLIST_TEXLIST;
			GameGearBackgroundParams[index].TexNumber = (b % 2 != 0) ? (9 - i * 2) : (7 + i * 2);
			GameGearBackgroundParams[index].UvTop.x = (b % 2 != 0) ? 1 : 0;
			GameGearBackgroundParams[index].UvTop.y = 0;
			GameGearBackgroundParams[index].UvDown.x = (b % 2 != 0) ? 0 : 1;
			GameGearBackgroundParams[index].UvDown.y = 1.0f;
			GameGearBackgroundParams[index].Top.x = 0 - startpoint + 256.0f * i + 640.0f * b + ((b % 2 != 0) ? 128.0f : 0.0f);
			GameGearBackgroundParams[index].Top.y = 0;
			index++;
			// Bottom
			GameGearBackgroundParams[index].Color = 0xFFFFFFFF;
			GameGearBackgroundParams[index].Priority = 0.3f;
			GameGearBackgroundParams[index].Size.x = 256.0f;
			GameGearBackgroundParams[index].Size.y = 256.0f;
			GameGearBackgroundParams[index].TexList = &GG_TEXLIST_TEXLIST;
			GameGearBackgroundParams[index].TexNumber = (b % 2 != 0) ? (10 - i * 2) : (8 + i * 2);
			GameGearBackgroundParams[index].UvTop.x = (b % 2 != 0) ? 1 : 0;;
			GameGearBackgroundParams[index].UvTop.y = 0;
			GameGearBackgroundParams[index].UvDown.x = (b % 2 != 0) ? 0 : 1;
			GameGearBackgroundParams[index].UvDown.y = 1.0f;
			GameGearBackgroundParams[index].Top.x = 0 - startpoint + 256.0f * i + 640.0f * b + ((b % 2 != 0) ? 128.0f : 0.0f);
			GameGearBackgroundParams[index].Top.y = 256.0f;
			index++;

		}
		// Right
		for (int d = 0; d < 2; d++)
		{
			// Top right
			GameGearBackgroundParams[index].Color = 0xFFFFFFFF;
			GameGearBackgroundParams[index].Priority = 0.3f;
			GameGearBackgroundParams[index].Size.x = 128.0f;
			GameGearBackgroundParams[index].Size.y = 128.0f;
			GameGearBackgroundParams[index].TexList = &GG_TEXLIST_TEXLIST;
			GameGearBackgroundParams[index].TexNumber = 1 + d;
			GameGearBackgroundParams[index].UvTop.x = (b % 2 != 0) ? 1 : 0;
			GameGearBackgroundParams[index].UvTop.y = 0;
			GameGearBackgroundParams[index].UvDown.x = (b % 2 != 0) ? 0 : 1;
			GameGearBackgroundParams[index].UvDown.y = 1.0f;
			GameGearBackgroundParams[index].Top.x = 0 - startpoint + 640.0f * b + ((b % 2 != 0) ? 0.0f : 512.0f);
			GameGearBackgroundParams[index].Top.y = 128.0f * d;
			index++;
			// Bottom right
			GameGearBackgroundParams[index].Color = 0xFFFFFFFF;
			GameGearBackgroundParams[index].Priority = 0.3f;
			GameGearBackgroundParams[index].Size.x = 128.0f;
			GameGearBackgroundParams[index].Size.y = 128.0f;
			GameGearBackgroundParams[index].TexList = &GG_TEXLIST_TEXLIST;
			GameGearBackgroundParams[index].TexNumber = 3 + d;
			GameGearBackgroundParams[index].UvTop.x = (b % 2 != 0) ? 1 : 0;
			GameGearBackgroundParams[index].UvTop.y = 0;
			GameGearBackgroundParams[index].UvDown.x = (b % 2 != 0) ? 0 : 1;
			GameGearBackgroundParams[index].UvDown.y = 1.0f;
			GameGearBackgroundParams[index].Top.x = 0 - startpoint + 640.0f * b + ((b % 2 != 0) ? 0.0f : 512.0f);
			GameGearBackgroundParams[index].Top.y = 128.0f * d + 256.0f;
			index++;
		}
	}
}

void DrawHoneycomb(NJS_POINT2COL* pointcol, Int n, Float pri, Uint32 attr)
{
	for (int i = 0; i < 6; i++)
		pointcol->p[i].x -= ((float)HorizontalResolution - 640) / 2;
	njDrawCircle2D(pointcol, n, pri, attr);
}

void GameGearTexmemlist_NoAlphaRejection(NJS_TEXTURE_VTX* a1, Int count, Uint32 gbix, Int flag)
{
	if (gbix == 1310100 || gbix == 1311000) // Pause menu shadow, corners
	{
		njTextureShadingMode(1);
		//PrintDebug("%d\n", gbix);
	}
	else
		njTextureShadingMode(2);
	njDrawTextureMemList(a1, count, gbix, flag);
}

void GameGear_Init()
{
	WriteCall((void*)0x701764, GameGearTexmemlist_NoAlphaRejection);
	HorizontalResolution_Float = (float)HorizontalResolution;
	GenerateGameGearBackground();
	WriteData<1>((char*)0x007032C3, 0x40); // 64 textures for background (game select screen)
	WriteData((AdvertiseBaseParam**)0x007032BB, GameGearBackgroundParams); // Replace background array (game select screen)
	WriteData((AdvertiseBaseParam**)0x006FFFBB, GameGearBackgroundParams); // Replace background array (controls screen)
	WriteData<1>((char*)0x006FFFC3, 0x40); // 64 textures for background (controls screen)
	// Honeycomb transition
	WriteData((float**)0x006FF5C8, &VerticalStretch);
	WriteData((float**)0x006FF5D9, &VerticalStretch);
	WriteData((float**)0x006FF5EB, &VerticalStretch);
	WriteData((float**)0x006FF603, &VerticalStretch);
	WriteData((float**)0x006FF611, &VerticalStretch);
	WriteData((float**)0x006FF632, &VerticalStretch);
	WriteData((float**)0x006FF657, &VerticalStretch);
	WriteData((float**)0x006FF4B1, &HorizontalResolution_Float);
	WriteCall((void*)0x006FF662, DrawHoneycomb);
}