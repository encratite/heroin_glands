#include <heroin/item.hpp>

extern std::string const item_classification_strings[] =
{
	"Helm",
	"Armor",
	"Shield",
	"Gloves",
	"Boots",
	"Belt",
	"Druid Pelt",
	"Barbarian Helm",
	"Paladin Shield",
	"Necromancer Shrunken Head",
	"Axe",
	"Mace",
	"Sword",
	"Dagger",
	"Throwing Weapon",
	"Javelin",
	"Spear",
	"Polearm",
	"Bow",
	"Crossbow",
	"Staff",
	"Wand",
	"Scepter",
	"Assassin Katar",
	"Sorceresss Orb",
	"Amazon Weapon",
	"Circlet",
	"Throwing Potion",
	"Quest Item",
	"Gem",
	"Rune",
	"Potion",
	"Charm",
	"Miscellaneous",
	"Jewel",
	"Amulet",
	"Gold",
	"Ring",
	"Ear"
};

namespace item_code
{
	extern std::string const
		gold = "gld";
}

bool item_type::operator<(item_type const & other) const
{
	return id < other.id;
}

item_property_type::item_property_type():
	value(0),

	minimum(0),
	maximum(0),
	length(0),

	level(0),
	character_class(0),
	skill(0),
	tab(0),

	monster(0),

	charges(0),
	maximum_charges(0),

	skill_chance(0),

	per_level(0)
{
}
