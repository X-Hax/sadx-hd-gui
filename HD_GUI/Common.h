#pragma once

#define TARGET_DYNAMIC(name) ((decltype(name##_r)*)name##_t->Target())
#define TARGET_STATIC(name) ((decltype(name##_r)*)name##_t.Target())

#define ReplacePVMX(a) helperFunctions.ReplaceFile("system\\" a ".PVM", "system\\" a "_HD.PVM")

void LoadSubtitleFont();

extern const HelperFunctions* helperFunctionsGlobal;