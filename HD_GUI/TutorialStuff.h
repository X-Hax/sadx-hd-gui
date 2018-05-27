#pragma once

struct TutorialScreenData
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

struct TutorialScreenItem
{
	char TexID;
	char unknown;
	__int16 XOffset;
	__int16 YOffset;
};

//Tutorial screens
DataArray(TutorialScreenData, TutoScreenSonic_E, 0x2BC3C30, 7);
DataArray(TutorialScreenData, TutoScreenSonic_J, 0x2BC3BA0, 7);
DataArray(TutorialScreenData, TutoScreenSonic_F, 0x2BC3CC0, 7);
DataArray(TutorialScreenData, TutoScreenSonic_G, 0x2BC3DE0, 7);
DataArray(TutorialScreenData, TutoScreenSonic_S, 0x2BC3D50, 7);
DataArray(TutorialScreenData, TutoScreenTails_E, 0x2BC4018, 7);
DataArray(TutorialScreenData, TutoScreenTails_J, 0x2BC3F78, 7);
DataArray(TutorialScreenData, TutoScreenTails_F, 0x2BC40A8, 7);
DataArray(TutorialScreenData, TutoScreenTails_S, 0x2BC4138, 7);
DataArray(TutorialScreenData, TutoScreenTails_G, 0x2BC41C8, 7);
DataArray(TutorialScreenData, TutoScreenKnuckles_E, 0x2BC4450, 7);
DataArray(TutorialScreenData, TutoScreenKnuckles_J, 0x2BC4398, 7);
DataArray(TutorialScreenData, TutoScreenKnuckles_F, 0x2BC44F0, 7);
DataArray(TutorialScreenData, TutoScreenKnuckles_G, 0x2BC4630, 7);
DataArray(TutorialScreenData, TutoScreenKnuckles_S, 0x2BC4590, 7);
DataArray(TutorialScreenData, TutoScreenAmy_E, 0x2BC4830, 7);
DataArray(TutorialScreenData, TutoScreenAmy_J, 0x2BC47A0, 7);
DataArray(TutorialScreenData, TutoScreenAmy_F, 0x2BC48C0, 7);
DataArray(TutorialScreenData, TutoScreenAmy_G, 0x2BC49E0, 7);
DataArray(TutorialScreenData, TutoScreenAmy_S, 0x2BC4950, 7);
DataArray(TutorialScreenData, TutoScreenBig_E, 0x2BC5158, 9);
DataArray(TutorialScreenData, TutoScreenBig_J, 0x2BC5090, 10);
DataArray(TutorialScreenData, TutoScreenBig_F, 0x2BC5210, 9);
DataArray(TutorialScreenData, TutoScreenBig_G, 0x2BC5380, 9);
DataArray(TutorialScreenData, TutoScreenBig_S, 0x2BC52C8, 9);
DataArray(TutorialScreenData, TutoScreenGamma_E, 0x2BC4C08, 8);
DataArray(TutorialScreenData, TutoScreenGamma_J, 0x2BC4B68, 8);
DataArray(TutorialScreenData, TutoScreenGamma_F, 0x2BC4CA8, 8);
DataArray(TutorialScreenData, TutoScreenGamma_G, 0x2BC4DE8, 8);
DataArray(TutorialScreenData, TutoScreenGamma_S, 0x2BC4D48, 8);

//Tutorial screen items
DataArray(TutorialScreenItem, TutorialLayout_Sonic_Page1_E, 0x02BC3ACE, 6);
DataArray(TutorialScreenItem, TutorialLayout_Sonic_Page1_J, 0x02BC3A8E, 5);
DataArray(TutorialScreenItem, TutorialLayout_SharedTailsKnucklesPage1_E, 0x2BC3E90, 4);
DataArray(TutorialScreenItem, TutorialLayout_SharedTailsKnucklesPage1_J, 0x2BC3E74, 3);
DataPointer(TutorialScreenItem, TutorialLayout_Tails_Page1_E, 0x2BC3EB2);
DataPointer(TutorialScreenItem, TutorialLayout_Tails_Page1_J, 0x2BC3EAA);
DataPointer(TutorialScreenItem, TutorialLayout_Knuckles_Page1_E, 0x2BC425E);
DataPointer(TutorialScreenItem, TutorialLayout_Knuckles_Page1_J, 0x2BC4256);
DataArray(TutorialScreenItem, TutorialLayout_AmyGamma_Page1_E, 0x2BC46E6, 4);
DataArray(TutorialScreenItem, TutorialLayout_AmyGamma_Page1_J, 0x2BC46D2, 3);
DataArray(TutorialScreenItem, TutorialLayout_Amy_Page1_E, 0x2BC4712, 2);
DataArray(TutorialScreenItem, TutorialLayout_Amy_Page1_J, 0x2BC4702, 2);
DataArray(TutorialScreenItem, TutorialLayout_BigPage1_E, 0x2BC4E8A, 4);
DataArray(TutorialScreenItem, TutorialLayout_BigPage1Part2_E, 0x2BC4EB6, 2);
DataArray(TutorialScreenItem, TutorialLayout_BigPage1Part2_J, 0x2BC4EA6, 2);
DataArray(TutorialScreenItem, TutorialLayout_BigPage5_E, 0x2BC4F46, 6);
DataArray(TutorialScreenItem, TutorialLayout_Gamma_Page1_E, 0x2BC4A7E, 2);
DataArray(TutorialScreenItem, TutorialLayout_Gamma_Page1_J, 0x2BC4A6E, 2);

