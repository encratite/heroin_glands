#include <iostream>
#include <sstream>

#include <heroin/item_data.hpp>
#include <heroin/utility.hpp>
#include <heroin/game.hpp>
#include <nil/array.hpp>

using namespace item_classification;

item_entry::item_entry()
{
}

item_entry::item_entry(std::string const & name, std::string const & type, item_classification_type classification, ulong width, ulong height, bool stackable, bool useable, bool throwable):
	name(name),
	type(type),
	classification(classification),
	width(width),
	height(height),
	stackable(stackable),
	useable(useable),
	throwable(throwable)
{
}

bool item_entry::is_armor() const
{
	switch(classification)
	{
		case helm:
		case armor:
		case shield:
		case gloves:
		case boots:
		case belt:
		case druid_pelt:
		case barbarian_helm:
		case paladin_shield:
		case necromancer_shrunken_head:
		case circlet:
			return true;
	}
	return false;
}

bool item_entry::is_weapon() const
{
	switch(classification)
	{

		case amazon_bow:
		case amazon_javelin:
		case amazon_spear:
		case assassin_katar:
		case axe:
		case bow:
		case club:
		case crossbow:
		case dagger:
		case hammer:
		case javelin:
		case mace:
		case polearm:
		case scepter:
		case sorceress_orb:
		case spear:
		case sword:
		case throwing_axe:
		case throwing_knife:
		case throwing_potion:
			return true;
	}
	return false;
}

std::string item_quality_to_string(item_quality_type quality)
{
	using namespace item_quality;

	switch(quality)
	{
	case inferior:
		return "Inferior";
	case normal:
		return "Normal";
	case superior:
		return "Superior";
	case magical:
		return "Magical";
	case set:
		return "Set";
	case rare:
		return "Rare";
	case unique:
		return "Unique";
	case crafted:
		return "Crafted";
	}
	return "Unknown";
}
