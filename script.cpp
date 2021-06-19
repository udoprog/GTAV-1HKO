#include "script.h"

const Hash Trevor = 2608926626u;
bool mod_active = true;
bool mod_training = true;
bool mod_needs_init = true;


void ScriptMain() {
	Ped current_ped;
	int max_health, health;
	int max_armour, armour;
	std::string mod_status;
	std::string hit_count_str;
	int hit_count = 0;

	while (true) {
		if (mod_active) {
			// initialize at start and if mod has been toggled on/off
			if (mod_needs_init) {
				current_ped = PLAYER::PLAYER_PED_ID();
				health = ENTITY::GET_ENTITY_HEALTH(current_ped);
				max_health = ENTITY::GET_ENTITY_MAX_HEALTH(current_ped);
				armour = PED::GET_PED_ARMOUR(current_ped);
				max_armour = PLAYER::GET_PLAYER_MAX_ARMOUR(PLAYER::PLAYER_ID());
				mod_needs_init = false;
				hit_count = 0;
			}

			Player player = PLAYER::PLAYER_ID();
			Ped new_ped = PLAYER::PLAYER_PED_ID();

			// ped switches on death, so this must be before the death check below.
			if (new_ped != current_ped) {
				current_ped = new_ped;
				health = ENTITY::GET_ENTITY_HEALTH(current_ped);
				max_health = ENTITY::GET_ENTITY_MAX_HEALTH(current_ped);
				armour = PED::GET_PED_ARMOUR(current_ped);
				max_armour = PLAYER::GET_PLAYER_MAX_ARMOUR(player);
			}

			// update current health and armour
			int current_health = ENTITY::GET_ENTITY_HEALTH(current_ped);
			int current_max_health = ENTITY::GET_ENTITY_MAX_HEALTH(current_ped);
			int current_armour = PED::GET_PED_ARMOUR(current_ped);
			int current_max_armour = PLAYER::GET_PLAYER_MAX_ARMOUR(player);

			// stop doing anything while dying
			if (PED::IS_PED_DEAD_OR_DYING(current_ped, true)) {
				WAIT(0);
				continue;
			}

			// deplete Trevor special ability
			if (PED::IS_PED_MODEL(current_ped, Trevor) && PLAYER::IS_SPECIAL_ABILITY_ACTIVE(player)) {
				PLAYER::SPECIAL_ABILITY_DEPLETE_METER(player, true);
			}

			// change max health if it changed.
			if (max_health != current_max_health) {
				max_health = current_max_health;
				// Check if max_health has been reduced
				if (health > max_health)
					health = max_health;
			}

			// change health if it increased
			if (current_health > health)
				health = current_health;

			// change max armour if it changed.
			if (max_armour != current_max_armour) {
				max_armour = current_max_armour;
				// Check if max_armour has been reduced
				if (armour > max_armour)
					armour = max_armour;
			}
			// change armour if it increased
			if (current_armour > armour)
				armour = current_armour;


			// Health drop check
			if (current_health < health) {
				// Kill the ped if not training
				if (mod_training)
					health = current_health;
				else
					ENTITY::SET_ENTITY_HEALTH(current_ped, 0);
				hit_count++;
			}

			// Armour drop check
			if (current_armour < armour) {
				// Kill the ped if not training
				if (mod_training)
					armour = current_armour;
				else
					ENTITY::SET_ENTITY_HEALTH(current_ped, 0);
				hit_count++;
			}
		}

		// Display on screen
		mod_status = std::string("1HKO") + (mod_training ? " (train)" : "") + ": " + (mod_active ? "ON" : "OFF");
		hit_count_str = std::string("Hits: ") + std::to_string(hit_count);
		DrawTextToScreen(mod_status.c_str(), 0.9f, 0.5f, 0.3f, eFont::FontChaletLondon, false, 255, 255, 255);
		DrawTextToScreen(hit_count_str.c_str(), 0.9f, 0.53f, 0.3f, eFont::FontChaletLondon, false, 255, 255, 255);

		WAIT(0);
	}
}

void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scan_code, BOOL is_extended, BOOL is_with_alt, BOOL was_down_before, BOOL is_up_now) {
	if (!is_up_now) {
		// activate 1HKO
		if (key == 0x4B) { // Key K
			mod_active = !mod_active;

			if (mod_active)
				mod_needs_init = true;
		}
		// activate training mode
		else if (key == 0x4A)
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