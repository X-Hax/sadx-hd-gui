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