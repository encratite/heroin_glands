#ifndef GAME_HPP
#define GAME_HPP

#include <string>

enum character_class_type
{
	character_class_amazon,
	character_class_assassin,
	character_class_necromancer,
	character_class_barbarian,
	character_class_paladin,
	character_class_sorceress,
	character_class_druid
};

enum game_difficulty_type
{
	game_difficulty_normal,
	game_difficulty_nightmare,
	game_difficulty_hell
};

std::string character_class_to_string(character_class_type character_class);

#endif
