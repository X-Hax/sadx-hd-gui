#pragma once

#define ReplacePVMX(a) helperFunctions.ReplaceFile("system\\" a ".PVM", "system\\" a "_HD.PVM")

void LoadSubtitleFont();

extern const HelperFunctions* helperFunctionsGlobal;