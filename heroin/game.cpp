#include <cmath>
#include <sstream>

#include <heroin/game.hpp>

entity::entity():
	initialised(false)
{
}

entity::entity(ulong id, ulong x, ulong y):
	initialised(true),
	id(id),
	location(x, y)
{
}

player::player():
	directory_known(false)
{
}

player::player(std::string const & name, ulong id, character_class_type character_class, ulong level):
	entity(id, 0, 0),
	name(name),
	character_class(character_class),
	level(level),
	directory_known(false)
{
}

player::player(std::string const & name, ulong id, character_class_type character_class, ulong level, ulong x, ulong y):
	entity(id, x, y),
	name(name),
	character_class(character_class),
	level(level),
	directory_known(true)
{
}

void player::set_mercenary(ulong new_mercenary_id)
{
	has_mercenary = true;
	mercenary_id = new_mercenary_id;
}

std::string character_class_to_string(character_class_type character_class)
{
	switch(character_class)
	{
		case character_class::amazon: return "Amazon";
		case character_class::assassin: return "Assassin";
		case character_class::necromancer: return "Necromancer";
		case character_class::barbarian: return "Barbarian";
		case character_class::paladin: return "Paladin";
		case character_class::sorceress: return "Sorceress";
		case character_class::druid: return "Druid";
		default: return "Unknown";
	}
}

coordinate::coordinate()
{
}

coordinate::coordinate(ulong x, ulong y):
	x(x),
	y(y)
{
}

std::string coordinate_string(ulong value)
{
	std::stringstream stream;
	stream.width(4);
	stream.fill('0');
	stream << std::hex << value;
	return stream.str();
}

std::string coordinate::string() const
{
	return "[" + coordinate_string(x) + ", " + coordinate_string(y) + "]";
}

bool coordinate::operator==(coordinate const & other) const
{
	return x == other.x && y == other.y;
}

double coordinate::distance(coordinate const & other) const
{
	double x_square = std::pow(static_cast<long>(x) - static_cast<long>(other.x), 2.0);
	double y_square = std::pow(static_cast<long>(y) - static_cast<long>(other.y), 2.0);
	double distance = std::sqrt(x_square + y_square);
	return distance;
}

npc_type::npc_type():
	entity()
{
}

npc_type::npc_type(ulong id, ulong type, ulong life, ulong x, ulong y):
	entity(id, x, y),
	type(type),
	life(life),
	has_flags(false),
	moving(false)
{
}
