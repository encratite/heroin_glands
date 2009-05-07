#include <iostream>
#include <sstream>
#include <nil/string.hpp>
#include <heroin/client.hpp>
#include <heroin/utility.hpp>

std::string d2_client::get_item_string(item_type const & item)
{
	//std::cout << "get_item_string" << std::endl;

	using namespace item_quality;

	std::stringstream stream;

	if(item.packet[0] == 0x9d)
		stream << "[9d] ";

	stream << "Action: " << item.action << ", ";
	if(item.ground)
		stream << "(ground " << item.x << ", " << item.y << ") ";
	else if(!item.unspecified_directory)
	{
		std::string container;
		switch(item.container)
		{
			case item_container::inventory:
				container = "Inventory";
				break;

			case item_container::cube:
				container = "Cube";
				break;

			case item_container::stash:
				container = "Stash";
				break;

			default:
				container = "Unknown container";
				break;
		}

		stream << "(" << container << " " << item.x << ", " << item.y << ") ";
	}

	if(item.ear)
		stream << "Ear of Level " << item.ear_level << " " << character_class_to_string(static_cast<character_class_type>(item.ear_character_class)) << " " << item.ear_name;
	else if(item.simple_item || item.gambling)
	{
		if(item.name.empty())
			stream << "Strange item: \"" << item.type << "\" " << get_data_string(item.packet);
		else
			stream << item.name;
	}
	else
	{
		std::string quality;
		if(item.quality != item_quality::not_applicable)
			quality = item_quality_to_string(item.quality) + " ";
		if(item.ethereal)
			stream << "Ethereal ";
		stream << "Level " << item.level << " " << quality << item.name;
		switch(item.quality)
		{
			case unique:
			{
				std::string unique_item;
				if(!client_data_manager.unique_items.get(item.unique_code, unique_item))
					unique_item = "Failed to look up unique item code";
				stream << ": " << unique_item;
				break;
			}

			case magical:
			{
				std::string prefix, suffix;
				if(item.prefix != 0)
				{
					if(!client_data_manager.magical_prefixes.get(item.prefix - 1, prefix))
						prefix = "Unknown prefix";
					else
						prefix += " ";
				}
				if(item.suffix != 0)
				{
					if(!client_data_manager.magical_suffixes.get(item.suffix - 1, suffix))
						suffix = "unknown suffix";
					else
						suffix = " " + suffix;
				}
				stream << ": " << prefix << item.name << suffix;
				break;
			}

			case crafted:
			case rare:
			{
				std::string prefix, suffix;
				if(!client_data_manager.rare_prefixes.get(item.prefix, prefix))
					prefix = "PrefixError";
				if(!client_data_manager.rare_suffixes.get(item.suffix, suffix))
					suffix = "SuffixError";
				stream << ": " << prefix << " " << suffix;
				break;
			}
		}

		//debugging
		/*
		if(item.simple_item)
			stream << " SimpleItem";
		if(item.is_armor)
			stream << " Armor";
		if(item.is_weapon)
			stream << " Weapon";
		*/

		if(item.is_armor)
			stream << ", Defense: " << item.defense;

		if(item.is_armor || item.is_weapon)
		{
			if(item.indestructible)
				stream << ", Indestructible";
			else
				stream << ", Durability: " << item.durability << "/" << item.maximum_durability;
		}

		if(item.in_socket)
			stream << " (in socket)" << std::endl;
		else if(item.has_sockets)
			stream << " (" << item.sockets << " socket" << (item.sockets > 1 ? "s" : "") << ", " << item.used_sockets << " used)";
	}

	for(std::size_t i = 0; i < item.properties.size(); i++)
	{
		stream << ", ";
		item_property_type const & item_property = item.properties[i];
		string_vector line;
		if(!client_data_manager.item_properties.get(static_cast<std::size_t>(item_property.stat), line))
		{
			stream << "Unknown stat";
			continue;
		}

		std::string const & name = line[0];
		ulong save_bits = nil::string::string_to_number<ulong>(line[1]);
		ulong save_param_bits = nil::string::string_to_number<ulong>(line[2]);

		stream << name;

		if(save_bits == 0 && save_param_bits == 0)
			continue;

		stream << ": ";

		using namespace item_stat;

		switch(item_property.stat)
		{
			case reanimate:
				stream << item_property.value << " (Monster ID " << item_property.monster;
				break;

			case elemental_skills:
				stream << item_property.value;
				break;

			case class_skills:
				stream << get_character_class_string(item_property.character_class + 1) << " Skills +" << item_property.value;
				break;

			case aura:
			case single_skill:
			case non_class_skill:
				stream << get_skill_name(item_property.skill) << " +" << item_property.value;
				break;

			case charged:
				stream << "Level " << item_property.level << " " << get_skill_name(item_property.skill) << " (" << item_property.charges << "/" << item_property.maximum_charges << ")";
				break;

			case skill_on_death:
				stream << item_property.skill_chance << "% chance of casting level " << item_property.level << " " << get_skill_name(item_property.skill) << " when killed";
				break;

			case skill_on_hit:
				stream << item_property.skill_chance << "% chance of casting level " << item_property.level << " " << get_skill_name(item_property.skill) << " when hitting an enemy";
				break;

			case skill_on_kill:
				stream << item_property.skill_chance << "% chance of casting level " << item_property.level << " " << get_skill_name(item_property.skill) << " when killing an enemy";
				break;

			case skill_on_level_up:
				stream << item_property.skill_chance << "% chance of casting level " << item_property.level << " " << get_skill_name(item_property.skill) << " on level-up";
				break;

			case skill_on_striking:
				stream << item_property.skill_chance << "% chance of casting level " << item_property.level << " " << get_skill_name(item_property.skill) << " on striking";
				break;

			case skill_when_struck:
				stream << item_property.skill_chance << "% chance of casting level " << item_property.level << " " << get_skill_name(item_property.skill) << " when struck";
				break;

			case skill_tab:
				stream << "Skill Tab " << item_property.tab << " " << item_property.character_class << " +" << item_property.value << std::endl;
				break;

			case defense_per_level:
			case enhanced_defense_per_level:
			case life_per_level:
			case mana_per_level:
			case maximum_damage_per_level:
			case maximum_enhanced_damage_per_level:
			case strength_per_level:
			case dexterity_per_level:
			case energy_per_level:
			case vitality_per_level:
			case attack_rating_per_level:
			case bonus_to_attack_rating_per_level:
			case maximum_cold_damage_per_level:
			case maximum_fire_damage_per_level:
			case maximum_lightning_damage_per_level:
			case maximum_poison_damage_per_level:
			case cold_resistance_per_level:
			case fire_resistance_per_level:
			case lightning_resistance_per_level:
			case poison_resistance_per_level:
			case cold_absorption_per_level:
			case fire_absorption_per_level:
			case lightning_absorption_per_level:
			case poison_absorption_per_level:
			case thorns_per_level:
			case extra_gold_per_level:
			case better_chance_of_getting_magic_item_per_level:
			case stamina_regeneration_per_level:
			case stamina_per_level:
			case damage_to_demons_per_level:
			case damage_to_undead_per_level:
			case attack_rating_against_demons_per_level:
			case attack_rating_against_undead_per_level:
			case crushing_blow_per_level:
			case open_wounds_per_level:
			case kick_damage_per_level:
			case deadly_strike_per_level:
			case find_gems_per_level:
				stream << item_property.per_level << "/level";
				break;

			case enhanced_maximum_damage:
			case enhanced_minimum_damage:
			case minimum_fire_damage:
			case minimum_lightning_damage:
			case minimum_magical_damage:
				stream << item_property.minimum << " - " << item_property.maximum;
				break;

			case minimum_cold_damage:
			case minimum_poison_damage:
				stream << item_property.minimum << " - " << item_property.maximum << " (" << item_property.length << " seconds)";
				break;

			default:
				stream << item_property.value;
				break;
		}
	}

	return stream.str();
}
