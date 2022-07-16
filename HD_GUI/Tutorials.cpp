#include <SADXModLoader.h>
#include "Common.h"
#include "Tutorials.h"

float PadManuXOffset_F = 175.0f;
float PadManuXOffset_General = 220.0f;
float PadManuXOffset_J = 200.0f;
float PadManuYOffset = 136.0f;
float PadManuYOffset2 = 105.0f;
float PadManuYMultiplier = 1.0f;

void Tutorials_Init(const char* path, const HelperFunctions& helperFunctions)
{
	// PVMs
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
	// Sonic
	// English
	TutoScreenXSonic_E[0].BoxScaleX = 390;
	TutoScreenXSonic_E[0].BoxScaleY = 144;
	TutoScreenXSonic_E[0].BoxX = 210;
	TutoScreenXSonic_E[4].BoxScaleY = 160;
	TutorialLayoutX_Sonic_Page1_E[1].XOffset = 136; // Rotate camera
	TutorialLayoutX_Sonic_Page1_E[1].YOffset = 0; // Rotate camera
	TutorialLayoutX_Sonic_Page1_E[2].XOffset = 136; // Maneuver character
	TutorialLayoutX_Sonic_Page1_E[2].YOffset = 24; // Maneuver character
	TutorialLayoutX_Sonic_Page1_E[4].XOffset = 136; // A button
	TutorialLayoutX_Sonic_Page1_E[4].YOffset = 64; // A button
	TutorialLayoutX_Sonic_Page1_E[4].TexID = 3;
	TutorialLayoutX_Sonic_Page1_E[5].TexID = 4;
	TutorialLayoutX_Sonic_Page1_E[5].XOffset = 136;
	TutorialLayoutX_Sonic_Page1_E[5].YOffset = 96;
	WriteData((__int16*)0x02BC3AE2, (__int16)2000); // Hide an extra item in controls page (all langs)
	// Japanese
	TutoScreenXSonic_J[0].BoxScaleX = 400;
	TutoScreenXSonic_J[0].BoxScaleY = 144;
	TutoScreenXSonic_J[0].BoxX = 200;
	TutoScreenXSonic_J[4].BoxScaleY = 160;
	TutorialLayoutX_Sonic_Page1_J[1].XOffset = 136; // Rotate camera
	TutorialLayoutX_Sonic_Page1_J[1].YOffset = 0; // Rotate camera
	TutorialLayoutX_Sonic_Page1_J[2].XOffset = 136; // Maneuver character
	TutorialLayoutX_Sonic_Page1_J[2].YOffset = 24; // Maneuver character
	TutorialLayoutX_Sonic_Page1_J[3].XOffset = 136; // A button
	TutorialLayoutX_Sonic_Page1_J[3].YOffset = 64; // A button
	TutorialLayoutX_Sonic_Page1_J[3].TexID = 3;
	TutorialLayoutX_Sonic_Page1_J[4].TexID = 4;
	TutorialLayoutX_Sonic_Page1_J[4].XOffset = 136;
	TutorialLayoutX_Sonic_Page1_J[4].YOffset = 96;
	// German
	TutoScreenXSonic_G[0].BoxScaleX = 380;
	TutoScreenXSonic_G[0].BoxScaleY = 144;
	TutoScreenXSonic_G[0].BoxX = 220;
	TutoScreenXSonic_G[3].BoxScaleY = 160;
	TutoScreenXSonic_G[4].BoxScaleY = 160;
	// French
	TutoScreenXSonic_F[4].BoxScaleY = 192;
	TutoScreenXSonic_F[0].BoxScaleX = 425;
	TutoScreenXSonic_F[0].BoxScaleY = 144;
	TutoScreenXSonic_F[0].BoxX = 175;
	// Spanish
	TutoScreenXSonic_S[0].BoxScaleX = 370;
	TutoScreenXSonic_S[0].BoxScaleY = 144;
	TutoScreenXSonic_S[0].BoxX = 230;
	TutoScreenXSonic_S[4].BoxScaleY = 192;
	// Tails
	// English
	TutoScreenXTails_E[0].BoxScaleX = 390;
	TutoScreenXTails_E[0].BoxScaleY = 144;
	TutoScreenXTails_E[0].BoxX = 210;
	TutoScreenXTails_E[4].BoxScaleY = 160;
	TutorialLayoutX_SharedTailsKnucklesPage1_E[0].XOffset = 136; // Rotate camera
	TutorialLayoutX_SharedTailsKnucklesPage1_E[0].YOffset = 0; // Rotate camera
	TutorialLayoutX_SharedTailsKnucklesPage1_E[1].XOffset = 136; // Maneuver character
	TutorialLayoutX_SharedTailsKnucklesPage1_E[1].YOffset = 24; // Maneuver character
	TutorialLayoutX_SharedTailsKnucklesPage1_E[3].XOffset = 136; // A button
	TutorialLayoutX_SharedTailsKnucklesPage1_E[3].YOffset = 96; // A button
	TutorialLayoutX_Tails_Page1_E.XOffset = 136; // Tail attack
	TutorialLayoutX_Tails_Page1_E.YOffset = 64; // Tail attack
	// Japanese
	TutoScreenXTails_J[0].BoxScaleX = 400;
	TutoScreenXTails_J[0].BoxScaleY = 144;
	TutoScreenXTails_J[0].BoxX = 200;
	TutoScreenXTails_J[4].BoxScaleY = 160;
	TutorialLayoutX_SharedTailsKnucklesPage1_J[0].XOffset = 136; // Rotate camera
	TutorialLayoutX_SharedTailsKnucklesPage1_J[0].YOffset = 0; // Rotate camera
	TutorialLayoutX_SharedTailsKnucklesPage1_J[1].XOffset = 136; // Maneuver character
	TutorialLayoutX_SharedTailsKnucklesPage1_J[1].YOffset = 24; // Maneuver character
	TutorialLayoutX_SharedTailsKnucklesPage1_J[2].XOffset = 136; // A button
	TutorialLayoutX_SharedTailsKnucklesPage1_J[2].YOffset = 96; // A button
	TutorialLayoutX_Tails_Page1_J.XOffset = 136; // Tail attack
	TutorialLayoutX_Tails_Page1_J.YOffset = 64; // Tail attack
	// French
	TutoScreenXTails_F[4].BoxScaleY = 192;
	TutoScreenXTails_F[0].BoxScaleX = 425;
	TutoScreenXTails_F[0].BoxScaleY = 144;
	TutoScreenXTails_F[0].BoxX = 175;
	// German
	TutoScreenXTails_G[0].BoxScaleX = 390;
	TutoScreenXTails_G[0].BoxScaleY = 144;
	TutoScreenXTails_G[0].BoxX = 210;
	TutoScreenXTails_G[3].BoxScaleY = 128;
	TutoScreenXTails_G[4].BoxScaleY = 160;
	TutoScreenXTails_G[0].BoxScaleX = 380;
	TutoScreenXTails_G[0].BoxScaleY = 144;
	TutoScreenXTails_G[0].BoxX = 220;
	WriteData((__int16*)0x02BC3E9E, (__int16)2000); // Hide an extra item in controls page
	// Spanish
	TutoScreenXTails_S[0].BoxScaleX = 370;
	TutoScreenXTails_S[0].BoxScaleY = 144;
	TutoScreenXTails_S[0].BoxX = 230;
	TutoScreenXTails_S[4].BoxScaleY = 192;
	// Knuckles
	// English
	TutoScreenXKnuckles_E[0].BoxScaleX = 390;
	TutoScreenXKnuckles_E[0].BoxScaleY = 144;
	TutoScreenXKnuckles_E[0].BoxX = 210;
	TutoScreenXKnuckles_E[4].BoxX = 180;
	TutoScreenXKnuckles_E[4].BoxScaleY = 128;
	TutoScreenXKnuckles_E[4].BoxScaleX = 425;
	TutoScreenXKnuckles_E[5].BoxScaleY = 160;
	TutorialLayoutX_Knuckles_Page1_E.XOffset = 136; // Punch attack
	TutorialLayoutX_Knuckles_Page1_E.YOffset = 64; // Punch attack
	WriteData<1>((char*)0x02BC4308, 0x03); // Number of items in Maximum Heat screen, remove unnecessary line
	// Japanese
	TutoScreenXKnuckles_J[0].BoxScaleX = 400;
	TutoScreenXKnuckles_J[0].BoxScaleY = 144;
	TutoScreenXKnuckles_J[0].BoxX = 200;
	TutoScreenXKnuckles_J[4].BoxX = 180;
	TutoScreenXKnuckles_J[4].BoxScaleY = 128;
	TutoScreenXKnuckles_J[4].BoxScaleX = 425;
	TutoScreenXKnuckles_J[5].BoxScaleY = 160;
	TutorialLayoutX_Knuckles_Page1_J.XOffset = 136; // Punch attack
	TutorialLayoutX_Knuckles_Page1_J.YOffset = 64; // Punch attack
	// German
	TutoScreenXKnuckles_G[0].BoxScaleX = 380;
	TutoScreenXKnuckles_G[0].BoxScaleY = 144;
	TutoScreenXKnuckles_G[0].BoxX = 220;
	TutoScreenXKnuckles_G[2].BoxScaleY = 128;
	TutoScreenXKnuckles_G[4].BoxX = 180;
	TutoScreenXKnuckles_G[4].BoxScaleX = 420;
	TutoScreenXKnuckles_G[4].BoxScaleY = 128;
	TutoScreenXKnuckles_G[5].BoxScaleY = 160;
	WriteData((__int16*)0x02BC42E0, (__int16)2000); // Hide an extra item in Climbing page
	// French
	TutoScreenXKnuckles_F[0].BoxScaleX = 430;
	TutoScreenXKnuckles_F[0].BoxScaleY = 144;
	TutoScreenXKnuckles_F[0].BoxX = 170;
	TutoScreenXKnuckles_F[1].BoxScaleX = 455;
	TutoScreenXKnuckles_F[1].BoxX = 145;
	TutoScreenXKnuckles_F[4].BoxScaleX = 475;
	TutoScreenXKnuckles_F[4].BoxX = 125;
	TutoScreenXKnuckles_F[4].BoxScaleY = 128;
	TutoScreenXKnuckles_F[5].BoxScaleY = 192;
	WriteData((__int16*)0x02BC3E9E, (__int16)2000); // Hide an extra item in Controls page
	WriteData((__int16*)0x02BC433A, (__int16)2000); // Hide an extra item in Maximum Heat page
	WriteData((__int16*)0x02BC4340, (__int16)2000); // Hide an extra item in Maximum Heat page
	// Spanish
	TutoScreenXKnuckles_S[0].BoxScaleX = 370;
	TutoScreenXKnuckles_S[0].BoxScaleY = 144;
	TutoScreenXKnuckles_S[0].BoxX = 230;
	TutoScreenXKnuckles_S[4].BoxScaleX = 415;
	TutoScreenXKnuckles_S[4].BoxX = 190;
	TutoScreenXKnuckles_S[4].BoxScaleY = 128;
	TutoScreenXKnuckles_S[5].BoxScaleY = 192;
	// Amy
	// English
	TutoScreenXAmy_E[0].BoxScaleX = 390;
	TutoScreenXAmy_E[0].BoxScaleY = 144;
	TutoScreenXAmy_E[0].BoxX = 210;
	TutoScreenXAmy_E[4].BoxScaleY = 160;
	TutorialLayoutX_AmyGamma_Page1_E[1].XOffset = 136; // Rotate camera
	TutorialLayoutX_AmyGamma_Page1_E[1].YOffset = 0; // Rotate camera
	TutorialLayoutX_AmyGamma_Page1_E[2].XOffset = 136; // Maneuver character
	TutorialLayoutX_AmyGamma_Page1_E[2].YOffset = 24; // Maneuver character
	TutorialLayoutX_Amy_Page1_E[0].XOffset = 136; // A
	TutorialLayoutX_Amy_Page1_E[0].YOffset = 96; // A
	TutorialLayoutX_Amy_Page1_E[1].XOffset = 136; // B
	TutorialLayoutX_Amy_Page1_E[1].YOffset = 64; // B
	// Japanese
	TutoScreenXAmy_J[0].BoxScaleX = 400;
	TutoScreenXAmy_J[0].BoxScaleY = 144;
	TutoScreenXAmy_J[0].BoxX = 200;
	TutoScreenXAmy_J[4].BoxScaleY = 160;
	TutorialLayoutX_AmyGamma_Page1_J[1].XOffset = 136; // Rotate camera
	TutorialLayoutX_AmyGamma_Page1_J[1].YOffset = 0; // Rotate camera
	TutorialLayoutX_AmyGamma_Page1_J[2].XOffset = 136; // Maneuver character
	TutorialLayoutX_AmyGamma_Page1_J[2].YOffset = 24; // Maneuver character
	TutorialLayoutX_Amy_Page1_J[0].XOffset = 136; // A
	TutorialLayoutX_Amy_Page1_J[0].YOffset = 96; // A
	TutorialLayoutX_Amy_Page1_J[1].XOffset = 136; // B
	TutorialLayoutX_Amy_Page1_J[1].YOffset = 64; // B
	// German
	TutoScreenXAmy_G[0].BoxScaleX = 380;
	TutoScreenXAmy_G[0].BoxScaleY = 144;
	TutoScreenXAmy_G[0].BoxX = 220;
	TutoScreenXAmy_G[4].BoxScaleY = 160;
	WriteData((__int16*)0x02BC46FA, (__int16)2000); // Hide an extra item in controls page
	// French
	TutoScreenXAmy_F[0].BoxScaleX = 425;
	TutoScreenXAmy_F[0].BoxScaleY = 144;
	TutoScreenXAmy_F[0].BoxX = 175;
	TutoScreenXAmy_F[4].BoxScaleY = 192;
	// Spanish
	TutoScreenXAmy_S[0].BoxScaleX = 370;
	TutoScreenXAmy_S[0].BoxScaleY = 144;
	TutoScreenXAmy_S[0].BoxX = 230;
	TutoScreenXAmy_S[4].BoxScaleY = 192;
	// Big
	// English
	TutoScreenXBig_E[0].BoxScaleX = 390;
	TutoScreenXBig_E[0].BoxScaleY = 144;
	TutoScreenXBig_E[0].BoxX = 210;
	TutoScreenXBig_E[4].BoxScaleY = 128;
	TutoScreenXBig_E[7].BoxScaleX = 420;
	TutoScreenXBig_E[7].BoxX = 180;
	TutorialLayoutX_BigPage1_E[1].XOffset = 136; // Rotate camera
	TutorialLayoutX_BigPage1_E[1].YOffset = 0; // Rotate camera
	TutorialLayoutX_BigPage1_E[2].XOffset = 136; // Maneuver character
	TutorialLayoutX_BigPage1_E[2].YOffset = 24; // Maneuver character
	TutorialLayoutX_BigPage1Part2_E[0].XOffset = 136; // A
	TutorialLayoutX_BigPage1Part2_E[0].YOffset = 96; // A
	TutorialLayoutX_BigPage1Part2_E[1].XOffset = 136; // B
	TutorialLayoutX_BigPage1Part2_E[1].YOffset = 64; // B
	TutorialLayoutX_BigPage5_E[2].TexID = 32; // Hide "Tugging the rod"
	TutorialLayoutX_BigPage5_E[3].YOffset = 64; // Move A up
	TutorialLayoutX_BigPage5_E[4].YOffset = 96; // Move B up
	// Japanese
	TutoScreenXBig_J[0].BoxScaleX = 400;
	TutoScreenXBig_J[0].BoxScaleY = 144;
	TutoScreenXBig_J[0].BoxX = 200;
	TutoScreenXBig_J[4].BoxScaleY = 160;
	TutoScreenXBig_J[7].BoxScaleX = 400;
	TutoScreenXBig_J[7].BoxX = 200;
	TutorialLayoutX_BigPage1Part2_J[0].XOffset = 136; // A
	TutorialLayoutX_BigPage1Part2_J[0].YOffset = 96; // A
	TutorialLayoutX_BigPage1Part2_J[1].XOffset = 136; // B
	TutorialLayoutX_BigPage1Part2_J[1].YOffset = 64; // B
	// German
	TutoScreenXBig_G[0].BoxScaleX = 395;
	TutoScreenXBig_G[0].BoxScaleY = 144;
	TutoScreenXBig_G[0].BoxX = 205;
	TutoScreenXBig_G[4].BoxScaleY = 128;
	WriteData((__int16*)0x02BC4E9E, (__int16)2000); // Hide an extra item in controls page
	// French
	TutoScreenXBig_F[0].BoxScaleX = 425;
	TutoScreenXBig_F[0].BoxScaleY = 144;
	TutoScreenXBig_F[0].BoxX = 175;
	TutoScreenXBig_F[4].BoxScaleY = 128;
	TutoScreenXBig_F[7].BoxScaleX = 445;
	TutoScreenXBig_F[7].BoxX = 155;
	// Spanish
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
	// Gamma
	// English
	TutoScreenXGamma_E[0].BoxScaleX = 390;
	TutoScreenXGamma_E[0].BoxScaleY = 144;
	TutoScreenXGamma_E[0].BoxX = 210;
	TutoScreenXGamma_E[4].BoxScaleY = 160;
	TutorialLayoutX_Gamma_Page1_E[0].XOffset = 136; // A
	TutorialLayoutX_Gamma_Page1_E[0].YOffset = 96; // A
	TutorialLayoutX_Gamma_Page1_E[1].XOffset = 136; // B
	TutorialLayoutX_Gamma_Page1_E[1].YOffset = 64; // B
	// Japanese
	TutoScreenXGamma_J[0].BoxScaleX = 400;
	TutoScreenXGamma_J[0].BoxScaleY = 144;
	TutoScreenXGamma_J[0].BoxX = 200;
	TutoScreenXGamma_J[4].BoxScaleY = 160;
	TutorialLayoutX_Gamma_Page1_J[0].XOffset = 136; // A
	TutorialLayoutX_Gamma_Page1_J[0].YOffset = 96; // A
	TutorialLayoutX_Gamma_Page1_J[1].XOffset = 136; // B
	TutorialLayoutX_Gamma_Page1_J[1].YOffset = 64; // B
	// German
	TutoScreenXGamma_G[4].BoxScaleY = 160;
	TutoScreenXGamma_G[0].BoxScaleX = 380;
	TutoScreenXGamma_G[0].BoxScaleY = 144;
	TutoScreenXGamma_G[0].BoxX = 220;
	// French
	TutoScreenXGamma_F[0].BoxScaleX = 425;
	TutoScreenXGamma_F[0].BoxScaleY = 144;
	TutoScreenXGamma_F[0].BoxX = 175;
	TutoScreenXGamma_F[1].BoxScaleY = 192;
	TutoScreenXGamma_F[4].BoxScaleY = 192;
	WriteData((__int16*)0x02BC4AE4, (__int16)2000); // Hide an extra item in the second page
	// Spanish
	TutoScreenXGamma_S[0].BoxScaleX = 370;
	TutoScreenXGamma_S[0].BoxScaleY = 144;
	TutoScreenXGamma_S[0].BoxX = 230;
	TutoScreenXGamma_S[4].BoxScaleY = 192;
}

void Tutorials_OnFrame()
{
	if (GetCharacterSelection() == 2)
		PadManuXOffset_F = 170;
	else
		PadManuXOffset_F = 175;
	switch (TextLanguage)
	{
	case 3:
		PadManuXOffset_General = 230;
		break;
	case 4:
		if (GetCharacterSelection() == 4)
			PadManuXOffset_General = 205;
		else
			PadManuXOffset_General = 220;
		break;
	default:
		PadManuXOffset_General = 205;
		break;
	}
}