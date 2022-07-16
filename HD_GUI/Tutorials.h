#pragma once

struct TutorialScreenDataX
{
	__int16 BoxX;
	__int16 BoxY;
	__int16 anonymous_2;
	__int16 BoxScaleX;
	__int16 BoxScaleY;
	__int16 anonymous_3;
	void *Pointer1;
	void *Pointer2;
};

struct TutorialScreenItemX
{
	char TexID;
	char unknown;
	__int16 XOffset;
	__int16 YOffset;
};

// Tutorial screens
DataArray(TutorialScreenDataX, TutoScreenXSonic_E, 0x2BC3C30, 7);
DataArray(TutorialScreenDataX, TutoScreenXSonic_J, 0x2BC3BA0, 7);
DataArray(TutorialScreenDataX, TutoScreenXSonic_F, 0x2BC3CC0, 7);
DataArray(TutorialScreenDataX, TutoScreenXSonic_G, 0x2BC3DE0, 7);
DataArray(TutorialScreenDataX, TutoScreenXSonic_S, 0x2BC3D50, 7);
DataArray(TutorialScreenDataX, TutoScreenXTails_E, 0x2BC4018, 7);
DataArray(TutorialScreenDataX, TutoScreenXTails_J, 0x2BC3F78, 7);
DataArray(TutorialScreenDataX, TutoScreenXTails_F, 0x2BC40A8, 7);
DataArray(TutorialScreenDataX, TutoScreenXTails_S, 0x2BC4138, 7);
DataArray(TutorialScreenDataX, TutoScreenXTails_G, 0x2BC41C8, 7);
DataArray(TutorialScreenDataX, TutoScreenXKnuckles_E, 0x2BC4450, 7);
DataArray(TutorialScreenDataX, TutoScreenXKnuckles_J, 0x2BC4398, 7);
DataArray(TutorialScreenDataX, TutoScreenXKnuckles_F, 0x2BC44F0, 7);
DataArray(TutorialScreenDataX, TutoScreenXKnuckles_G, 0x2BC4630, 7);
DataArray(TutorialScreenDataX, TutoScreenXKnuckles_S, 0x2BC4590, 7);
DataArray(TutorialScreenDataX, TutoScreenXAmy_E, 0x2BC4830, 7);
DataArray(TutorialScreenDataX, TutoScreenXAmy_J, 0x2BC47A0, 7);
DataArray(TutorialScreenDataX, TutoScreenXAmy_F, 0x2BC48C0, 7);
DataArray(TutorialScreenDataX, TutoScreenXAmy_G, 0x2BC49E0, 7);
DataArray(TutorialScreenDataX, TutoScreenXAmy_S, 0x2BC4950, 7);
DataArray(TutorialScreenDataX, TutoScreenXBig_E, 0x2BC5158, 9);
DataArray(TutorialScreenDataX, TutoScreenXBig_J, 0x2BC5090, 10);
DataArray(TutorialScreenDataX, TutoScreenXBig_F, 0x2BC5210, 9);
DataArray(TutorialScreenDataX, TutoScreenXBig_G, 0x2BC5380, 9);
DataArray(TutorialScreenDataX, TutoScreenXBig_S, 0x2BC52C8, 9);
DataArray(TutorialScreenDataX, TutoScreenXGamma_E, 0x2BC4C08, 8);
DataArray(TutorialScreenDataX, TutoScreenXGamma_J, 0x2BC4B68, 8);
DataArray(TutorialScreenDataX, TutoScreenXGamma_F, 0x2BC4CA8, 8);
DataArray(TutorialScreenDataX, TutoScreenXGamma_G, 0x2BC4DE8, 8);
DataArray(TutorialScreenDataX, TutoScreenXGamma_S, 0x2BC4D48, 8);

// Tutorial screen items
DataArray(TutorialScreenItemX, TutorialLayoutX_Sonic_Page1_E, 0x02BC3ACE, 6);
DataArray(TutorialScreenItemX, TutorialLayoutX_Sonic_Page1_J, 0x02BC3A8E, 5);
DataArray(TutorialScreenItemX, TutorialLayoutX_SharedTailsKnucklesPage1_E, 0x2BC3E90, 4);
DataArray(TutorialScreenItemX, TutorialLayoutX_SharedTailsKnucklesPage1_J, 0x2BC3E74, 3);
DataPointer(TutorialScreenItemX, TutorialLayoutX_Tails_Page1_E, 0x2BC3EB2);
DataPointer(TutorialScreenItemX, TutorialLayoutX_Tails_Page1_J, 0x2BC3EAA);
DataPointer(TutorialScreenItemX, TutorialLayoutX_Knuckles_Page1_E, 0x2BC425E);
DataPointer(TutorialScreenItemX, TutorialLayoutX_Knuckles_Page1_J, 0x2BC4256);
DataArray(TutorialScreenItemX, TutorialLayoutX_AmyGamma_Page1_E, 0x2BC46E6, 4);
DataArray(TutorialScreenItemX, TutorialLayoutX_AmyGamma_Page1_J, 0x2BC46D2, 3);
DataArray(TutorialScreenItemX, TutorialLayoutX_Amy_Page1_E, 0x2BC4712, 2);
DataArray(TutorialScreenItemX, TutorialLayoutX_Amy_Page1_J, 0x2BC4702, 2);
DataArray(TutorialScreenItemX, TutorialLayoutX_BigPage1_E, 0x2BC4E8A, 4);
DataArray(TutorialScreenItemX, TutorialLayoutX_BigPage1Part2_E, 0x2BC4EB6, 2);
DataArray(TutorialScreenItemX, TutorialLayoutX_BigPage1Part2_J, 0x2BC4EA6, 2);
DataArray(TutorialScreenItemX, TutorialLayoutX_BigPage5_E, 0x2BC4F46, 6);
DataArray(TutorialScreenItemX, TutorialLayoutX_Gamma_Page1_E, 0x2BC4A7E, 2);
DataArray(TutorialScreenItemX, TutorialLayoutX_Gamma_Page1_J, 0x2BC4A6E, 2);