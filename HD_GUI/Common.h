#pragma once
#include <d3dx8.h>

#define ReplacePVMX(a) helperFunctions.ReplaceFile("system\\" a ".PVM", "system\\" a "_HD.PVM")

void LoadSubtitleFont();
void LoadFont();
void DrawSubtitleText();

extern const HelperFunctions* helperFunctionsGlobal;
extern ID3DXFont* font;
extern bool FontLoaded;