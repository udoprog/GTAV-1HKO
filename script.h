#pragma once

#include "natives.h"
#include "types.h"
#include "enums.h"

#include "main.h"

#include <string>

struct PedStats {
	int ped;
	int health;
	int max_health;
	int armour;
	int max_armour;
};

struct DisplaySettings {
	float offset_x = 0.9f;
	float offset_y = 0.55f;
	float size = 0.3f;
	eFont font = eFont::FontChaletLondon;
	int red = 255;
	int green = 255;
	int blue = 255;
};

void ScriptMain();

void SetStatVarsFromPed(int ped_id, int player_id, PedStats & stats);

void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scan_code, BOOL is_extended, BOOL is_with_alt, BOOL was_down_before, BOOL is_up_now);
void DrawTextToScreen(const std::string & text, float x, float y, float scale, eFont font, bool align_right, int red, int green, int blue);

void ReloadSettings();
void LoadSettings(const std::string & filename, DisplaySettings & s);
void SaveSettings(const std::string & filename, const DisplaySettings & s);