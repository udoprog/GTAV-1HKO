#include "script.h"

#include <mutex>
#include <fstream>
#include <sstream>
#include <filesystem>

const Hash Trevor = 2608926626u;
const std::string settings_filename = "1HKO.ini";
DisplaySettings disp_s;

bool mod_active = true;
bool mod_training = true;
bool mod_needs_init = true;
bool mod_manual = false;
std::mutex hit_count_mutex;
int hit_count = 0;

bool control_key_pressed = false;


void ScriptMain() {
	PedStats previous = { 0, 0, 0, 0, 0 };
	PedStats current = { 0, 0, 0, 0, 0 };

	// load/save settings
	ReloadSettings();

	std::string mod_status, hit_count_str, display_str;
	bool trevor_ability_was_locked = false;
	
	const std::string s_mod("1HKO"), s_death("Death"), s_deaths("Deaths"), s_hit("Hit"), s_hits("Hits");

	while (true) {
		if (mod_active) {
			Player player = PLAYER::PLAYER_ID();
			Ped new_ped = PLAYER::PLAYER_PED_ID();

			// initialize at start and if mod has been toggled on/off
			if (mod_needs_init) {
				std::lock_guard<std::mutex> lock(hit_count_mutex);
				SetStatVarsFromPed(new_ped, player, previous);
				mod_needs_init = false;
				mod_manual = false;
				hit_count = 0;
			}

			// update current health and armour
			SetStatVarsFromPed(new_ped, player, current);

			// ped switches on death, so this must be before the death check below.
			if (current.ped != previous.ped) {
				SetStatVarsFromPed(current.ped, player, previous);
			}

			// stop doing anything while dying
			if (PED::IS_PED_DEAD_OR_DYING(current.ped, true)) {
				WAIT(0);
				continue;
			}

			// disable Trevor special ability
			if (PED::IS_PED_MODEL(current.ped, Trevor) && PLAYER::IS_SPECIAL_ABILITY_UNLOCKED(Trevor)) {
				PLAYER::SPECIAL_ABILITY_LOCK(Trevor);
				trevor_ability_was_locked = true;
			}

			// change max health if it changed
			if (previous.max_health != current.max_health) {
				previous.max_health = current.max_health;
				// check if max_health has been reduced
				if (previous.health > previous.max_health)
					previous.health = previous.max_health;
			}
			// change health if it increased
			if (current.health > previous.health)
				previous.health = current.health;

			// change max armour if it changed
			if (previous.max_armour != current.max_armour) {
				previous.max_armour = current.max_armour;
				// check if max_armour has been reduced
				if (previous.armour > previous.max_armour)
					previous.armour = previous.max_armour;
			}
			// change armour if it increased
			if (current.armour > previous.armour)
				previous.armour = current.armour;

			
			// health drop check
			if (current.health < previous.health) {
				std::lock_guard<std::mutex> lock(hit_count_mutex);

				// kill the ped if not training
				if (mod_training)
					previous.health = current.health;
				else
				{
					ENTITY::SET_ENTITY_HEALTH(current.ped, 0);
					PED::SET_PED_ARMOUR(current.ped, 0);
					// in case ped number is reused after death
					previous.health = 0;
					previous.armour = 0;
				}
				hit_count++;
			}
			// armour drop check
			if (current.armour < previous.armour) {
				std::lock_guard<std::mutex> lock(hit_count_mutex);

				// kill the ped if not training
				if (mod_training)
					previous.armour = current.armour;
				else
				{
					ENTITY::SET_ENTITY_HEALTH(current.ped, 0);
					PED::SET_PED_ARMOUR(current.ped, 0);
					// in case ped number is reused after death
					previous.health = 0;
					previous.armour = 0;
				}
				hit_count++;
			}
		}
		else
		{
			// unlock trevor special ability only if it was locked by the mod
			if (trevor_ability_was_locked) {
				if (!PLAYER::IS_SPECIAL_ABILITY_UNLOCKED(Trevor)) {
					PLAYER::SPECIAL_ABILITY_UNLOCK(Trevor);
					trevor_ability_was_locked = false;
				}
			}
		}

		// display on screen
		mod_status = s_mod + (mod_training ? " (train)" : "") + ": " + (mod_active ? "ON" : "OFF");
		hit_count_str = (mod_training ? (hit_count > 0 ? s_hits : s_hit) : (hit_count > 0 ? s_deaths : s_death))
						+ (mod_manual ? " (manual)" : "") + ": " + std::to_string(hit_count);
		display_str = mod_status + "\n" + hit_count_str;

		DrawTextToScreen(display_str.c_str(), disp_s.offset_x, disp_s.offset_y, disp_s.size,
			disp_s.font, false,
			disp_s.red, disp_s.green, disp_s.blue);

		WAIT(0);
	}
}

void SetStatVarsFromPed(int ped_id, int player_id, PedStats & stats) {
	stats.ped = ped_id;

	// health
	stats.health = ENTITY::GET_ENTITY_HEALTH(ped_id);
	stats.max_health = ENTITY::GET_ENTITY_MAX_HEALTH(ped_id);

	// armour
	stats.armour = PED::GET_PED_ARMOUR(ped_id);
	stats.max_armour = PLAYER::GET_PLAYER_MAX_ARMOUR(player_id);
}

void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scan_code, BOOL is_extended, BOOL is_with_alt, BOOL was_down_before, BOOL is_up_now) {
	if (is_up_now) {
		if (key == VK_CONTROL)
			control_key_pressed = false;
	}
	else {
		if (key == VK_CONTROL)
			control_key_pressed = true;
		// decrease hit count
		else if (control_key_pressed && key == 0x4A) { // Ctrl + Key J
			if (hit_count > 0) {
				std::lock_guard<std::mutex> lock(hit_count_mutex);
				mod_manual = true;
				hit_count--;
			}
		}
		// increase hit count
		else if (control_key_pressed && key == 0x4B) { // Ctrl + Key K
			std::lock_guard<std::mutex> lock(hit_count_mutex);
			mod_manual = true;
			hit_count++;
		}
		// reload settings
		else if (control_key_pressed && key == 0x4D) { // Ctrl + Key M
			ReloadSettings();
		}
		// activate 1HKO
		else if (key == 0x4B) { // Key K
			mod_active = !mod_active;	

			if (mod_active)
				mod_needs_init = true;
		}
		// activate training mode
		else if (key == 0x4A) // Key J
			mod_training = !mod_training;
	}
}

void DrawTextToScreen(const std::string & text, float x, float y, float scale, eFont font, bool align_right, int red, int green, int blue) {
	// WARNING: Only the first 100 calls to this function are displayed!
	UI::SET_TEXT_FONT((int)font);
	UI::SET_TEXT_SCALE(scale, scale);
	UI::SET_TEXT_COLOUR(red, green, blue, 255);
	UI::SET_TEXT_WRAP(0.0, 1.0);
	UI::SET_TEXT_CENTRE(false);
	UI::SET_TEXT_RIGHT_JUSTIFY(align_right);
	UI::SET_TEXT_DROPSHADOW(2, 2, 0, 0, 0);
	UI::SET_TEXT_EDGE(1, 0, 0, 0, 205);
	UI::_SET_TEXT_ENTRY((char *) "STRING");
	UI::_ADD_TEXT_COMPONENT_STRING3((char *) text.c_str());
	UI::_DRAW_TEXT(x, y);
}

void ReloadSettings() {
	if (std::filesystem::exists(settings_filename))
		LoadSettings(settings_filename, disp_s);
	else
		SaveSettings(settings_filename, disp_s);
}

void LoadSettings(const std::string & filename, DisplaySettings & s) {
	std::ifstream file(filename);
	std::string line;

	while (std::getline(file, line)) {
		std::istringstream current(line.substr(line.find("=") + 1));

		if (line.find("offset_x") != -1)
			current >> s.offset_x;
		else if (line.find("offset_y") != -1)
			current >> s.offset_y;
		else if (line.find("size") != -1)
			current >> s.size;
		else if (line.find("red") != -1)
			current >> s.red;
		else if (line.find("green") != -1)
			current >> s.green;
		else if (line.find("blue") != -1)
			current >> s.blue;
	}
}

void SaveSettings(const std::string & filename, const DisplaySettings & s) {
	std::ofstream file(filename);
	file << "offset_x = " << s.offset_x << std::endl;
	file << "offset_y = " << s.offset_y << std::endl;
	file << "size = " << s.size << std::endl;
	file << "red = " << s.red << std::endl;
	file << "green = " << s.green << std::endl;
	file << "blue = " << s.blue << std::endl;
}