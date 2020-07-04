#include <SADXModLoader.h>
#include "Scaling.h"

float ResolutionScaleX = 1.0f;
float ResolutionScaleY = 1.0f;
float mainscale = 1.0f;
float subscale = 1.0f;
bool is640 = true;
float zerof = 0;
float ggsel_size = 47.0f;
float ggsel_topleft = 136.0f;

GameGearTextureData GameGearTextureDataArray[128];

void AddGameGearTexture(AdvertiseBaseParam* param, char array, char index)
{
	for (int i = 0; i < LengthOfArray(GameGearTextureDataArray); i++)
	{
		if (GameGearTextureDataArray[i].size_x == 0 && GameGearTextureDataArray[i].top_x == 0)
		{
			GameGearTextureDataArray[i].size_x = param->Size.x;
			GameGearTextureDataArray[i].size_y = param->Size.y;
			GameGearTextureDataArray[i].top_x = param->Top.x;
			GameGearTextureDataArray[i].top_y = param->Top.y;
			GameGearTextureDataArray[i].array = array;
			GameGearTextureDataArray[i].index = index;
			//PrintDebug("Added GG texture %d, size %f %f, top %f %f, array %d, index %d\n", i, GameGearTextureDataArray[i].size_x, GameGearTextureDataArray[i].size_y, GameGearTextureDataArray[i].top_x, GameGearTextureDataArray[i].top_y, array, index);
			return;
		}
	}
}

void AddGameGearTextures_all()
{
	//Textures
	AdvertiseBaseParam* pointer;
	for (int u = 0; u < LengthOfArray(GameGearTextures); u++)
	{
		pointer = (AdvertiseBaseParam*)GameGearTextures[u].address;
		for (int i = 0; i < GameGearTextures[u].count; i++)
		{
			AddGameGearTexture(&pointer[i], u, i);
		}
	}
}

Uint8 GetGameGearTextureIndex(int array, int index)
{
	for (int i = 0; i < LengthOfArray(GameGearTextureDataArray); i++)
	{
		if (GameGearTextureDataArray[i].array == array && GameGearTextureDataArray[i].index == index) return i;
	}
	return 0;
}

void ScaleGameGear_all()
{
	//Textures
	AdvertiseBaseParam* pointer;
	char index = 0;
	for (int u = 0; u < LengthOfArray(GameGearTextures); u++)
	{
		pointer = (AdvertiseBaseParam*)GameGearTextures[u].address;
		for (int i = 0; i < GameGearTextures[u].count; i++)
		{
			index = GetGameGearTextureIndex(u, i);
			pointer[i].Size.x = GameGearTextureDataArray[index].size_x * mainscale;
			pointer[i].Size.y = GameGearTextureDataArray[index].size_y * mainscale;
			pointer[i].Top.x = (float)HorizontalResolution / 2.0f - (320.0f - GameGearTextureDataArray[index].top_x) * mainscale;
			pointer[i].Top.y = (float)VerticalResolution / 2.0f - (240.0f - GameGearTextureDataArray[index].top_y) * mainscale;
		}
	}
	//Honeycomb
	WriteData((float**)0x006FF5C8, &subscale);
	WriteData((float**)0x006FF5D9, &subscale);
	WriteData((float**)0x006FF5EB, &subscale);
	WriteData((float**)0x006FF603, &subscale);
	WriteData((float**)0x006FF611, &subscale);
	WriteData((float**)0x006FF632, &subscale);
	WriteData((float**)0x006FF657, &subscale);
	//Selection rectangle (pause)
	WriteData((char*)0x006FE265, char(int(46.0f * mainscale)));
	//Selection rectangle (main)
	ggsel_size = mainscale * 47.0f;
	ggsel_topleft = (float)VerticalResolution / 2.0f - (240.0f - 136.0f) * mainscale;
	WriteData((float**)0x006FDC70, &ggsel_size);
	WriteData((float**)0x006FDC76, &ggsel_topleft);
	WriteData((float**)0x006FDC7C, &zerof);
	WriteData((float**)0x006FDC88, &zerof);
	//Emulator
	WriteData((float**)0x006FEB63, &zerof);
	WriteData((float**)0x006FEB83, &zerof);
	WriteData((float**)0x006FEB91, &zerof);
	WriteData((float**)0x006FEB9D, &zerof);
	//General
	WriteData((float**)0x00701447, &zerof);
	WriteData((float**)0x00701453, &zerof);
	WriteData((float**)0x00701469, &zerof);
	WriteData((float**)0x00701478, &zerof);
}

void ReinitScaling()
{
	//PrintDebug("Render device reset\n");
	//PrintDebug("New resolution: %d x %d\n", HorizontalResolution, VerticalResolution);
	ResolutionScaleX = (float)HorizontalResolution / 640.0f;
	ResolutionScaleY = (float)VerticalResolution / 480.0f;
	if (HorizontalResolution == 640 && VerticalResolution == 480) is640 = true; else is640 = false;
	if (ResolutionScaleX >= ResolutionScaleY)
	{
		mainscale = ResolutionScaleY;
		subscale = ResolutionScaleX;
	}
	else
	{
		mainscale = ResolutionScaleX;
		subscale = ResolutionScaleY;
	}
}