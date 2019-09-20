#include "script.h"

Hash Trevor = 2608926626u;

void ScriptMain() {
	Ped current_ped = PLAYER::PLAYER_PED_ID();
	int health = ENTITY::GET_ENTITY_HEALTH(current_ped);
	int max_health = ENTITY::GET_ENTITY_MAX_HEALTH(current_ped);

	while (true) {
		Player player = PLAYER::PLAYER_ID();
		Ped new_ped = PLAYER::GET_PLAYER_PED(player);

		int current_health = ENTITY::GET_ENTITY_HEALTH(current_ped);
		int current_max_health = ENTITY::GET_ENTITY_MAX_HEALTH(current_ped);

		// ped switches on death, so this must be before the death check below.
		if (new_ped != current_ped) {
			current_ped = new_ped;
			health = current_health;
			max_health = current_max_health;
		}

		if (PED::IS_PED_DEAD_OR_DYING(current_ped, true)) {
			WAIT(0);
			continue;
		}

		if (PED::IS_PED_MODEL(current_ped, Trevor) && PLAYER::IS_SPECIAL_ABILITY_ACTIVE(player)) {
			PLAYER::SPECIAL_ABILITY_DEPLETE_METER(player, true);
		}

		// change current health if max health has changed.
		if (max_health != current_max_health) {
			health = current_health;
			max_health = current_max_health;
		}

		if (current_health < health) {
			ENTITY::SET_ENTITY_HEALTH(current_ped, 0);
		}

		WAIT(0);
	}
}
