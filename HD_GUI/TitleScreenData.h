#pragma once

#pragma warning(push)
#pragma warning(disable: 4838)

DataPointer(char, IsTitleScreenReplaced, 0x510350);

struct TitleScreenData
{
	int f0;
	void* field_4;
	void* field_8;
	float field_C;
	char gap_10[4];
	int field_14;
	int field_18;
	char gap_1c[8];
	_DWORD dword24;
	_DWORD dword28;
	char f2C[4];
	_DWORD dword30;
	float float34;
	char byte38;
	char _padding[3];
	char field_3C[36];
};

#pragma warning(pop)