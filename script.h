#pragma once

#include "natives.h"
#include "types.h"
#include "enums.h"

#include "main.h"

#include <string>

void ScriptMain();
void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scan_code, BOOL is_extended, BOOL is_with_alt, BOOL was_down_before, BOOL is_up_now);
void DrawTextToScreen(const std::string & text, float x, float y, float scale, eFont font, bool align_right, int red, int green, int blue);