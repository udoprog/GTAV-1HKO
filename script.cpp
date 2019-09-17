#include "script.h"

Hash Trevor = 2608926626u;

void ScriptMain() {
	Ped playerPed = PLAYER::PLAYER_PED_ID();

	Ped current = playerPed;
	int health = ENTITY::GET_ENTITY_HEALTH(current);
	int armor = PED::GET_PED_ARMOUR(current);
	bool just_died = false;

	while (true) {
		Player player = PLAYER::PLAYER_ID();
		Ped newPed = PLAYER::GET_PLAYER_PED(player);

		if (newPed != current) {
			current = newPed;
			health = ENTITY::GET_ENTITY_HEALTH(current);
			armor = PED::GET_PED_ARMOUR(current);
		}

		if (PED::IS_PED_DEAD_OR_DYING(current, true)) {
			just_died = true;
			WAIT(0);
			continue;
		}

		if (just_died) {
			health = ENTITY::GET_ENTITY_HEALTH(current);
			armor = PED::GET_PED_ARMOUR(current);
			just_died = false;
		}

		if (PED::IS_PED_MODEL(current, Trevor) && PLAYER::IS_SPECIAL_ABILITY_ACTIVE(player)) {
			PLAYER::SPECIAL_ABILITY_DEPLETE_METER(player, true);
			WAIT(0);
			continue;
		}

		int current_health = ENTITY::GET_ENTITY_HEALTH(current);
		int current_armor = PED::GET_PED_ARMOUR(current);

		if (current_health < health || current_armor < armor) {
			ENTITY::SET_ENTITY_HEALTH(current, 0);
		}

		WAIT(0);
	}
}
