#include <iostream>
#include <string>

#include <heroin/client.hpp>
#include <heroin/bit_reader.hpp>
#include <heroin/utility.hpp>
#include <heroin/item_data.hpp>
#include <nil/array.hpp>
#include <nil/string.hpp>

void d2_client::item_action(std::string const & data)
{
	item_type item = parse_item(data);
	/*
	if(item.has_sockets)
		std::cout << get_char_array_string(data) << std::endl;
	*/
	if(debugging)
		std::cout << get_item_string(item) << std::endl;

	if(!item.ground && !item.unspecified_directory)
	{
		switch(item.container)
		{
			case item_container::inventory:
				inventory.add(item);
				break;

			case item_container::cube:
				cube.add(item);
				break;

			case item_container::stash:
				stash.add(item);
				break;
			
			/*
			default:
				std::cout << "Unknown container encountered while parsing an item: " << item.name << " (" << item.type << "), container " << item.container << std::endl;
				break;
			*/
		}
	}
}

//pretty much stolen from Awesome-O



item_type d2_client::parse_item(std::string const & data)
{
	//print_data(data);
	item_type item;
	item.packet = data;

	try
	{
		bit_reader reader(data);
		ulong packet = reader.read(8);
		item.action = reader.read(8);
		item.category = reader.read(8);
		ulong valid_size = reader.read(8);
		item.id = reader.read(32);

		if(packet == 0x9d)
			reader.read(40);

		item.equipped = reader.read_bool();
		reader.read_bool();
		reader.read_bool();
		item.in_socket = reader.read_bool();
		item.identified = reader.read_bool();
		reader.read_bool();
		item.switched_in = reader.read_bool();
		item.switched_out = reader.read_bool();

		item.broken = reader.read_bool();
		reader.read_bool();
		item.potion = reader.read_bool();
		item.has_sockets = reader.read_bool();
		reader.read_bool();
		item.in_store = reader.read_bool();
		item.not_in_a_socket = reader.read_bool();
		reader.read_bool();

		item.ear = reader.read_bool();
		item.start_item = reader.read_bool();
		reader.read_bool();
		reader.read_bool();
		reader.read_bool();
		item.simple_item = reader.read_bool();
		item.ethereal = reader.read_bool();
		reader.read_bool();

		item.personalised = reader.read_bool();
		item.gambling = reader.read_bool();
		item.rune_word = reader.read_bool();
		reader.read(5);

		item.version = static_cast<item_version_type>(reader.read(8));

		reader.read(2);
		ulong destination = reader.read(3);

		item.ground = (destination == 0x03);

		if(item.ground)
		{
			item.x = reader.read(16);
			item.y = reader.read(16);
		}
		else
		{
			item.directory = reader.read(4);
			item.x = reader.read(4);
			item.y = reader.read(3);
			item.container = static_cast<item_container_type>(reader.read(4));
		}

		item.unspecified_directory = false;

		if(item.action == item_action::add_to_shop || item.action == item_action::remove_from_shop)
		{
			long container = static_cast<long>(item.container);
			container |= 0x80;
			if(container & 1)
			{
				container--; //remove first bit
				item.y += 8;
			}
			item.container = static_cast<item_container_type>(container);
		}
		else if(item.container == item_container::unspecified)
		{
			if(item.directory == equipment_directory::not_applicable)
			{
				if(item.in_socket)
					//y is ignored for this container type, x tells you the index
					item.container = item_container::item;
				else if(item.action == item_action::put_in_belt || item.action == item_action::remove_from_belt)
				{
					item.container = item_container::belt;
					item.y = item.x / 4;
					item.x %= 4;
				}
			}
			else
				item.unspecified_directory = true;
		}

		if(item.ear)
		{
			item.ear_character_class = static_cast<character_class_type>(reader.read(3));
			item.ear_level = reader.read(7);
			item.ear_name = reader.string();
			return item;
		}

		char code_bytes[4];
		for(std::size_t i = 0; i < nil::countof(code_bytes); i++)
			code_bytes[i] = static_cast<char>(reader.read(8));
		code_bytes[3] = 0;

		item.type = std::string(code_bytes);

		item_entry entry;
		if(!client_data_manager.item_data.get(item.type, entry))
		{
			std::cout << "Failed to look up item in item data table" << std::endl;
			return item;
		}

		item.name = entry.name;
		item.width = entry.width;
		item.height = entry.height;

		item.is_gold = (item.type == item_code::gold);

		if(item.is_gold)
		{
			bool big_pile = reader.read_bool();
			if(big_pile)
				item.amount = reader.read(32);
			else
				item.amount = reader.read(12);
			return item;
		}

		item.used_sockets = reader.read(3);

		if(item.simple_item || item.gambling)
			return item;

		item.level = reader.read(7);

		item.quality = static_cast<item_quality_type>(reader.read(4));

		item.has_graphic = reader.read_bool();;
		if(item.has_graphic)
			item.graphic = reader.read(3);

		item.has_colour = reader.read_bool();
		if(item.has_colour)
			item.colour = reader.read(11);

		if(item.identified)
		{
			switch(item.quality)
			{
			case item_quality::inferior:
				item.prefix = reader.read(3);
				break;

			case item_quality::superior:
				item.superiority = static_cast<superior_item_class_type>(reader.read(3));
				break;

			case item_quality::magical:
				item.prefix = reader.read(11);
				item.suffix = reader.read(11);
				break;

			case item_quality::crafted:
			case item_quality::rare:
				item.prefix = reader.read(8) - 156;
				item.suffix = reader.read(8) - 1;
				if(debugging)
				{
					std::cout << "Rare prefix: " << item.prefix << std::endl;
					std::cout << "Rare suffix: " << item.suffix << std::endl;
					std::cout << get_char_array_string(data) << std::endl;
				}
				break;

			case item_quality::set:
				item.set_code = reader.read(12);
				break;

			case item_quality::unique:
				if(item.type != "std") //standard of heroes exception?
					item.unique_code = reader.read(12);
				break;
			}
		}

		if(item.quality == item_quality::rare || item.quality == item_quality::crafted)
		{
			for(ulong i = 0; i < 3; i++)
			{
				if(reader.read_bool())
					item.prefixes.push_back(reader.read(11));
				if(reader.read_bool())
					item.suffixes.push_back(reader.read(11));
			}
		}

		if(item.rune_word)
		{
			item.runeword_id = reader.read(12);
			item.runeword_parameter = reader.read(4);
		}

		if(item.personalised)
			item.personalised_name = reader.string();

		item.is_armor = entry.is_armor();
		item.is_weapon = entry.is_weapon();

		if(item.is_armor)
			item.defense = reader.read(11) - 10;

		if(entry.throwable)
		{
			reader.read(9);
			reader.read(17);
		}
		//special case: indestructible phase blade
		else if(item.type == "7cr")
			reader.read(8);
		else if(item.is_armor || item.is_weapon)
		{
			item.maximum_durability = reader.read(8);
			item.indestructible = item.maximum_durability == 0;
			/*
			if(!item.indestructible)
			{
				item.durability = reader.read(8);
				reader.read_bool();
			}
			*/

			//D2Hackit always reads it, hmmm. Appears to work.

			item.durability = reader.read(8);
			reader.read_bool();
		}

		if(item.has_sockets)
			item.sockets = reader.read(4);

		if(entry.stackable)
		{
			if(entry.useable)
				reader.read(5);

			item.amount = reader.read(9);
		}

		if(!item.identified)
			return item;

		if(item.quality == item_quality::set)
			ulong set_mods = reader.read(5);

		//reader.debugging = debugging;

		while(true)
		{
			//if(debugging)
			//	std::cout << "Reading stat ID" << std::endl;

			ulong stat_id = reader.read(9);

			if(stat_id == 0x1ff)
			{
				//if(debugging)
				//	std::cout << "Stat terminator encountered" << std::endl;
				break;
			}

			item_property_type item_property;
			process_item_stat(stat_id, reader, item_property);
			item.properties.push_back(item_property);
		}
	}
	catch(nil::exception & exception)
	{
		std::cout << "Error occured: " << exception.get_message() << std::endl;
		std::cout << get_item_string(item) << std::endl;
		//print_data(data);
		std::cout << get_char_array_string(data) << std::endl;
		std::cin.get();
	}
	return item;
}

void d2_client::process_item_stat(unsigned stat_id, bit_reader & reader, item_property_type & item_property)
{
	using namespace item_stat;

	item_stat_type stat = static_cast<item_stat_type>(stat_id);
	item_property.stat = stat;

	string_vector line;
	if(!client_data_manager.item_properties.get(stat_id, line))
	{
		std::cout << "Invalid item stat ID: " << stat_id << std::endl;
		throw nil::exception("Failed to retrieve item stat data");
	}
	else if(line.size() != 4)
	{
		std::cout << "Invalid entry for item stat ID " << stat_id << std::endl;
		throw nil::exception("Invalid item stat token count detected");
	}

	std::string stat_name = line[0];
	ulong save_bits = nil::string::string_to_number<ulong>(line[1]);
	ulong save_param_bits = nil::string::string_to_number<ulong>(line[2]);
	//ulong signed_entry = nil::string::string_to_number<ulong>(line[3]);
	long save_add = nil::string::string_to_number<long>(line[3]);

	//std::cout << stat_name << " (" << stat_id << ")" << std::endl;

	if(save_param_bits > 0)
	{
		switch(stat)
		{
			case reanimate:
			{
				item_property.monster = reader.read(save_param_bits);
				item_property.value = reader.read(save_bits);
				return;
			}

			case elemental_skills:
			{
				//wtf is element?
				ulong element = reader.read(save_param_bits);
				item_property.value = reader.read(save_bits);
				return;
			}

			case class_skills:
			{
				item_property.character_class = reader.read(save_param_bits);
				item_property.value = reader.read(save_bits);
				return;
			}

			case aura:
			{
				item_property.skill = reader.read(save_param_bits);
				item_property.value = reader.read(save_bits);
				return;
			}

			case single_skill:
			case non_class_skill:
			{
				item_property.skill = reader.read(save_param_bits);
				item_property.value = reader.read(save_bits);
				return;
			}

			case charged:
			{
				item_property.level = reader.read(6);
				item_property.skill = reader.read(10);
				item_property.charges = reader.read(8);
				item_property.maximum_charges = reader.read(8);
				return;
			}

			case skill_on_death:
			case skill_on_hit:
			case skill_on_kill:
			case skill_on_level_up:
			case skill_on_striking:
			case skill_when_struck:
			{
				item_property.level = reader.read(6);
				item_property.skill = reader.read(10);
				item_property.skill_chance = reader.read(save_bits);
				return;
			}

			case skill_tab:
			{
				item_property.tab = reader.read(3);
				item_property.character_class = reader.read(3);
				ulong unknown = reader.read(10);
				item_property.value = reader.read(save_bits);
				return;
			}

			default:
				throw nil::exception("This makes no fucking sense");
		}
	}

	switch(stat)
	{
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
		{
			item_property.per_level = reader.read(save_bits);
			return;
		}
	}

	switch(stat_id)
	{
		case enhanced_maximum_damage:
		case enhanced_minimum_damage:
		{
			item_property.minimum = reader.read(save_bits);
			item_property.maximum = reader.read(save_bits);
			return;
		}

		case minimum_fire_damage:
		{
			item_property.minimum = reader.read(save_bits);
			item_property.maximum = reader.read(get_save_bits(static_cast<ulong>(maximum_fire_damage)));
			return;
		}

		case minimum_lightning_damage:
		{
			item_property.minimum = reader.read(save_bits);
			item_property.maximum = reader.read(get_save_bits(static_cast<ulong>(maximum_lightning_damage)));
			return;
		}

		case minimum_magical_damage:
		{
			item_property.minimum = reader.read(save_bits);
			item_property.maximum = reader.read(get_save_bits(static_cast<ulong>(maximum_magical_damage)));
			return;
		}

		case minimum_cold_damage:
		{
			item_property.minimum = reader.read(save_bits);
			item_property.maximum = reader.read(get_save_bits(static_cast<ulong>(maximum_cold_damage)));
			item_property.length = reader.read(get_save_bits(static_cast<ulong>(cold_damage_length)));
			return;
		}

		case minimum_poison_damage:
		{
			item_property.minimum = reader.read(save_bits);
			item_property.maximum = reader.read(get_save_bits(static_cast<ulong>(maximum_poison_damage)));
			item_property.length = reader.read(get_save_bits(static_cast<ulong>(poison_damage_length)));
			return;
		}

		case repairs_durability:
		case replenishes_quantity:
		{
			item_property.value = reader.read(save_bits);
			return;
		}

		default:
		{
			item_property.value = reader.read(save_bits) - save_add;
			return;
		}
	}
}

ulong d2_client::get_save_bits(unsigned stat_id)
{
	string_vector line;
	if(!client_data_manager.item_properties.get(stat_id, line))
	{
		std::cout << "Unable to find data for stat " << stat_id << std::endl;
		throw nil::exception("Unable to get save bits");
	}
	return nil::string::string_to_number<ulong>(line[1]);
}
