#pragma once

#include "natives.h"
#include "types.h"
#include "enums.h"

#include "main.h"

#include <string>

void ScriptMain();
void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow);
void DrawTextToScreen(const std::string & text, float x, float y, float scale, eFont font, bool alignRight, int red, int green, int blue);