#pragma once

struct AdvertiseBaseParam
{
	NJS_TEXLIST* TexList;
	int TexNumber;
	NJS_POINT2 Top;
	NJS_POINT2 Size;
	float Priority;
	unsigned int Color;
	NJS_POINT2 UvTop;
	NJS_POINT2 UvDown;
};

struct GameGearTextureArray
{
	int address;
	int count;
};

struct GameGearTextureData
{
	char array;
	char index;
	float top_x;
	float top_y;
	float size_x;
	float size_y;
};

GameGearTextureArray GameGearTextures[] = {
	{ 0x87C348, 3 },
	{ 0x87ECD0, 31 },
	{ 0x87E0C8, 24 },
	{ 0x87CAA8, 6 },
	{ 0x87F358, 17 },
	{ 0x87F9A0, 7 },
	{ 0x87C490, 15 },
	{ 0x87E6C8, 9 },
	{ 0x87F2E8, 1 },
	{ 0x87F700, 2 },
	{ 0x87F790, 2 },
};