#include "game.hpp"

std::string character_class_to_string(character_class_type character_class)
{
	switch(character_class)
	{
		case character_class_amazon: return "Amazon";
		case character_class_assassin: return "Assassin";
		case character_class_necromancer: return "Necromancer";
		case character_class_barbarian: return "Barbarian";
		case character_class_paladin: return "Paladin";
		case character_class_sorceress: return "Sorceress";
		case character_class_druid: return "Druid";
		default: return "Unknown";
	}
}