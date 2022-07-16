#include <SADXModLoader.h>
#include "Common.h"

#define ReplacePNG_MissionE(a) do { \
	_snprintf_s(pathbuf, LengthOfArray(pathbuf), "%s\\textures\\pvr_mission_en\\index.txt", path); \
	helperFunctions.ReplaceFile("system\\" a ".PVR", pathbuf); \
} while (0)

#define ReplacePNG_MissionJ(a) do { \
	_snprintf_s(pathbuf, LengthOfArray(pathbuf), "%s\\textures\\pvr_mission_jp\\index.txt", path); \
	helperFunctions.ReplaceFile("system\\" a ".PVR", pathbuf); \
} while (0)

#define ReplacePNG_MissionF(a) do { \
	_snprintf_s(pathbuf, LengthOfArray(pathbuf), "%s\\textures\\pvr_mission_fr\\index.txt", path); \
	helperFunctions.ReplaceFile("system\\" a ".PVR", pathbuf); \
} while (0)

#define ReplacePNG_GoalRing(a) do { \
	_snprintf_s(pathbuf, LengthOfArray(pathbuf), "%s\\textures\\pvr_mission_goalring\\index.txt", path); \
	helperFunctions.ReplaceFile("system\\" a ".PVR", pathbuf); \
} while (0)

void Mission_Init(const char* path, const HelperFunctions & helperFunctions)
{
	char pathbuf[MAX_PATH];
	HMODULE GoalRing = GetModuleHandle(L"GoalRing");
	// English mission PVRs
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
	// Japanese mission PVRs
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
		// Japanese
		ReplacePNG_MissionJ("MISSION_S_2MIN");
		ReplacePNG_MissionJ("MISSION_S_2MINH");
		ReplacePNG_MissionJ("MISSION_S_3MIN");
		ReplacePNG_MissionJ("MISSION_S_4MIN");
		ReplacePNG_MissionJ("MISSION_S_4MINH");
		ReplacePNG_MissionJ("MISSION_S_5MIN");
		ReplacePNG_MissionJ("MISSION_S_BOX");
		ReplacePNG_MissionJ("MISSION_S_RINGBOX");
		ReplacePNG_MissionJ("MISSION_T_BOX");
		// English
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
		// Japanese
		ReplacePNG_GoalRing("MISSION_S_2MIN");
		ReplacePNG_GoalRing("MISSION_S_2MINH");
		ReplacePNG_GoalRing("MISSION_S_3MIN");
		ReplacePNG_GoalRing("MISSION_S_4MIN");
		ReplacePNG_GoalRing("MISSION_S_4MINH");
		ReplacePNG_GoalRing("MISSION_S_5MIN");
		ReplacePNG_GoalRing("MISSION_S_BOX");
		ReplacePNG_GoalRing("MISSION_S_RINGBOX");
		ReplacePNG_GoalRing("MISSION_T_BOX");
		// English
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
	// French mission PVRs
	ReplacePNG_MissionF("MISSION_A_BALRING_F");
	ReplacePNG_MissionF("MISSION_A_BALZERO_F");
	ReplacePNG_MissionF("MISSION_A_FIN_F");
	ReplacePNG_MissionF("MISSION_A_HOT_F");
	ReplacePNG_MissionF("MISSION_A_TWIN_F");
	ReplacePNG_MissionF("MISSION_BIG_1K_F");
	ReplacePNG_MissionF("MISSION_BIG_2K_F");
	ReplacePNG_MissionF("MISSION_BIG_FROG_F");
	ReplacePNG_MissionF("MISSION_G_103RING_F");
	ReplacePNG_MissionF("MISSION_G_103_F");
	ReplacePNG_MissionF("MISSION_G_104RING_F");
	ReplacePNG_MissionF("MISSION_G_104_F");
	ReplacePNG_MissionF("MISSION_G_105RING_F");
	ReplacePNG_MissionF("MISSION_G_105_F");
	ReplacePNG_MissionF("MISSION_G_EME_F");
	ReplacePNG_MissionF("MISSION_G_FIN_F");
	ReplacePNG_MissionF("MISSION_G_FROGRING_F");
	ReplacePNG_MissionF("MISSION_G_FROG_F");
	ReplacePNG_MissionF("MISSION_G_HOT_F");
	ReplacePNG_MissionF("MISSION_G_RED_F");
	ReplacePNG_MissionF("MISSION_G_SONICDRING_F");
	ReplacePNG_MissionF("MISSION_G_SONICD_F");
	ReplacePNG_MissionF("MISSION_G_WIN_F");
	ReplacePNG_MissionF("MISSION_K_1MIN_F");
	ReplacePNG_MissionF("MISSION_K_2MIN_F");
	ReplacePNG_MissionF("MISSION_K_3EME_F");
	ReplacePNG_MissionF("MISSION_K_NOHINT_F");
	ReplacePNG_MissionF("MISSION_S_BOX25MIN_F");
	ReplacePNG_MissionF("MISSION_S_BOX2MIN_F");
	ReplacePNG_MissionF("MISSION_S_BOX3MIN_F");
	ReplacePNG_MissionF("MISSION_S_BOX45MIN_F");
	ReplacePNG_MissionF("MISSION_S_BOX4MIN_F");
	ReplacePNG_MissionF("MISSION_S_BOX5MIN_F");
	ReplacePNG_MissionF("MISSION_S_BOX_F");
	ReplacePNG_MissionF("MISSION_S_EGGC_F");
	ReplacePNG_MissionF("MISSION_S_EMECASINO_F");
	ReplacePNG_MissionF("MISSION_S_EMESNOW_F");
	ReplacePNG_MissionF("MISSION_S_EMEWIND_F");
	ReplacePNG_MissionF("MISSION_S_FEGG_F");
	ReplacePNG_MissionF("MISSION_S_ISEKI_F");
	ReplacePNG_MissionF("MISSION_S_RINGBOX_F");
	ReplacePNG_MissionF("MISSION_S_TAILS_F");
	ReplacePNG_MissionF("MISSION_T_BOX_F");
	ReplacePNG_MissionF("MISSION_T_EMECASINO_F");
	ReplacePNG_MissionF("MISSION_T_EMESNOW_F");
	ReplacePNG_MissionF("MISSION_T_EMEWIND_F");
	ReplacePNG_MissionF("MISSION_T_FASTEGG_F");
	ReplacePNG_MissionF("MISSION_T_FASTSONIC_F");
	ReplacePNG_MissionF("MISSION_T_MISS_F");
	ReplacePNG_MissionF("MISSION_T_RINGEGG_F");
	ReplacePNG_MissionF("MISSION_T_RINGSONIC_F");
}