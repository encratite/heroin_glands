#ifndef GAME_HPP
#define GAME_HPP

#include <string>

#include <nil/types.hpp>


//unused
namespace monster_modifier
{
	enum monster_modifier_type
	{
		none,
		rndname,
		hpmultiply,
		light,
		leveladd,
		strong,
		fast,
		curse,
		resist,
		fire,
		poisondead,
		durieldead,
		bloodraven,
		rage,
		spcdamage,
		partydead,
		champion,
		lightning,
		cold,
		hireable,
		scarab,
		killself,
		questcomplete,
		poisonhit,
		thief,
		manahit,
		teleport,
		spectralhit,
		stoneskin,
		multishot,
		aura,
		goboom,
		firespike_explode,
		suicideminion_explode,
		ai_after_death,
		shatter_on_death,
		ghostly,
		fanatic,
		possessed,
		berserk,
		worms_on_death,
		always_run_ai,
		lightningdeath,
	};
}

namespace character_class
{
	enum character_class_type
	{
		amazon,
		sorceress,
		necromancer,
		paladin,
		barbarian,
		druid,
		assassin
	};
}

namespace game_difficulty
{
	enum game_difficulty_type
	{
		normal,
		nightmare,
		hell
	};
}

enum act_type
{
	act_i,
	act_ii,
	act_iii,
	act_iv,
	act_v
};

using character_class::character_class_type;
using game_difficulty::game_difficulty_type;

struct coordinate
{
	ulong x, y;

	coordinate();
	coordinate(ulong x, ulong y);
	std::string string() const;

	bool operator==(coordinate const & other) const;
	double distance(coordinate const & other) const;
};

struct entity
{
	bool initialised;
	ulong id;
	coordinate location;

	entity();
	entity(ulong id, ulong x, ulong y);
};

struct player: public entity
{
	std::string name;
	bool has_mercenary;
	ulong mercenary_id;
	bool directory_known;
	character_class_type character_class;
	ulong level;

	player();
	player(std::string const & name, ulong id, character_class_type character_class, ulong level);
	player(std::string const & name, ulong id, character_class_type character_class, ulong level, ulong x, ulong y);
	void set_mercenary(ulong new_mercenary_id);
};

struct npc_type: public entity
{
	std::string name;

	ulong type;
	ulong life;
	coordinate target_location;

	bool moving;
	bool running;

	bool has_flags;

	bool flag1;
	bool flag2;
	bool super_unique;
	bool is_minion;
	bool flag5;

	ulong super_unique_id;
	
	npc_type();
	npc_type(ulong id, ulong type, ulong life, ulong x, ulong y);
};

std::string character_class_to_string(character_class_type character_class);

#endif
