#include <SADXModLoader.h>
#include "Common.h"

#define ReplaceTexDC(pvm_name, tex_name, gbix, width, height) do { \
_snprintf_s(pathbuf_pvm, LengthOfArray(pathbuf_pvm), "%s_DC", pvm_name);\
_snprintf_s(pathbuf_png, LengthOfArray(pathbuf_png), "%s\\replacetex\\%s\\%s.png", path, pvm_name, tex_name);\
helperFunctions.ReplaceTexture(pathbuf_pvm, tex_name, pathbuf_png, gbix, width, height);\
} while (0)

void TexturesDC_Init(const char* path, const HelperFunctions& helperFunctions)
{
	char pathbuf_pvm[MAX_PATH];
	char pathbuf_png[MAX_PATH];
	ReplaceTexDC("AVA_VMSSEL", "MS_TX", 365016, 512, 32);
	ReplaceTexDC("AVA_VMSSEL_E", "MS_TX_e", 10000445, 256, 32);
	ReplaceTexDC("EMBLEM", "sonic_emblm01", 4270, 64, 64);
	ReplaceTexDC("FISHING", "bighyouji", 361006, 256, 256);
	ReplaceTexDC("FISHING", "castgauge0", 361002, 128, 256);
	ReplaceTexDC("FISHING", "castgauge1", 361003, 64, 32);
	ReplaceTexDC("FISHING", "hit0", 361008, 128, 128);
	ReplaceTexDC("FISHING", "hit1", 361009, 128, 128);
	ReplaceTexDC("FISHING", "hyouji_g", 361007, 16, 16);
	ReplaceTexDC("KNU_EFF", "emerader0", 1351, 64, 64);
	ReplaceTexDC("KNU_EFF", "emerader1", 1352, 64, 64);
	ReplaceTexDC("KNU_EFF", "emerader2", 1353, 64, 64);
	ReplaceTexDC("KNU_EFF", "emerader3", 1354, 64, 64);
	ReplaceTexDC("KNU_EFF", "emerader4", 1355, 64, 64);
	ReplaceTexDC("KNU_EFF", "emerader5", 1356, 64, 64);
	ReplaceTexDC("KNU_EFF", "emerader6", 1357, 64, 64);
	ReplaceTexDC("KNU_EFF", "emerader7", 1358, 64, 64);
	ReplaceTexDC("KNU_EFF", "emerader8", 1359, 64, 64);
	ReplaceTexDC("KNU_EFF", "emerader_n", 1350, 64, 64);	
	ReplaceTexDC("OBJ_MINI_CART", "cart_lap0", 324038, 64, 32);
	ReplaceTexDC("OBJ_MINI_CART", "cart_lap1", 324039, 16, 16);
	ReplaceTexDC("OBJ_MINI_CART", "cart_lap2", 324040, 16, 16);
	ReplaceTexDC("OBJ_MINI_CART", "cart_lap3", 324041, 16, 16);
	ReplaceTexDC("OBJ_MINI_CART", "timetuika", 324042, 256, 16);
	ReplaceTexDC("OBJ_MINI_CART", "hyoji_head_cart", 324043, 64, 64);
	ReplaceTexDC("OBJ_MINI_CART", "hyoji_lap1", 324044, 64, 32);
	ReplaceTexDC("OBJ_MINI_CART", "hyoji_lap2", 324045, 64, 32);
	ReplaceTexDC("OBJ_MINI_CART", "hyoji_lap3", 324046, 64, 32);
	ReplaceTexDC("OBJ_MINI_CART", "hyoji_lap4", 324047, 64, 32);
	ReplaceTexDC("OBJ_REGULAR", "1upknuck", 4000, 64, 64);
	ReplaceTexDC("OBJ_REGULAR", "1upamy", 4001, 64, 64);
	ReplaceTexDC("OBJ_REGULAR", "1upbig", 4002, 64, 64);
	ReplaceTexDC("OBJ_REGULAR", "1up102", 4003, 64, 64);
	ReplaceTexDC("OBJ_REGULAR", "1uptails", 4004, 64, 64);
	ReplaceTexDC("OBJ_REGULAR", "item_1up", 4031, 64, 64);
	ReplaceTexDC("OBJ_REGULAR", "item_ring10", 4032, 64, 64);
	ReplaceTexDC("OBJ_REGULAR", "item_ring5", 4033, 64, 64);
	ReplaceTexDC("OBJ_REGULAR", "item_ringq", 4034, 64, 64);
	ReplaceTexDC("OBJ_REGULAR", "item_speed", 4035, 64, 64);
	ReplaceTexDC("OBJ_REGULAR", "item_tbarria", 4071, 64, 64);
	ReplaceTexDC("OBJ_REGULAR", "item_nbarria", 4072, 64, 64);
	ReplaceTexDC("OBJ_REGULAR", "item_muteki", 4073, 64, 64);
	ReplaceTexDC("OBJ_REGULAR", "item_explosion", 4074, 64, 64);
	ReplaceTexDC("OBJ_REGULAR", "item_ringnum", 3250000, 64, 64);
	ReplaceTexDC("OBJ_REGULAR", "1upmetal", 710226, 64, 64);
	ReplaceTexDC("SHOOTING0", "st_128s_hpgeji", 305004, 128, 128);
}