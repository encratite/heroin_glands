#include <iostream>
#include <map>
#include <heroin/data_manager.hpp>
#include <heroin/utility.hpp>
#include <nil/file.hpp>
#include <nil/string.hpp>
#include <nil/array.hpp>

item_data_type::item_data_type()
{
}

item_data_type::item_data_type(std::string const & file)
{
	typedef std::map<std::string, item_classification_type> classification_map_type;
	using namespace item_classification;

	classification_map_type classification_map;
	classification_map["Amazon Bow"] = amazon_bow;
	classification_map["Amazon Javelin"] = amazon_javelin;
	classification_map["Amazon Spear"] = amazon_spear;
	classification_map["Amulet"] = amulet;
	classification_map["Antidote Potion"] = antidote_potion;
	classification_map["Armor"] = armor;
	classification_map["Arrows"] = arrows;
	classification_map["Assassin Katar"] = assassin_katar;
	classification_map["Axe"] = axe;
	classification_map["Barbarian Helm"] = barbarian_helm;
	classification_map["Belt"] = belt;
	classification_map["Body Part"] = body_part;
	classification_map["Bolts"] = bolts;
	classification_map["Boots"] = boots;
	classification_map["Bow"] = bow;
	classification_map["Circlet"] = circlet;
	classification_map["Club"] = club;
	classification_map["Crossbow"] = crossbow;
	classification_map["Dagger"] = dagger;
	classification_map["Druid Pelt"] = druid_pelt;
	classification_map["Ear"] = ear;
	classification_map["Elixir"] = elixir;
	classification_map["Gem"] = gem;
	classification_map["Gloves"] = gloves;
	classification_map["Gold"] = gold;
	classification_map["Grand Charm"] = grand_charm;
	classification_map["Hammer"] = hammer;
	classification_map["Health Potion"] = health_potion;
	classification_map["Helm"] = helm;
	classification_map["Herb"] = herb;
	classification_map["Javelin"] = javelin;
	classification_map["Jewel"] = jewel;
	classification_map["Key"] = key;
	classification_map["Large Charm"] = large_charm;
	classification_map["Mace"] = mace;
	classification_map["Mana Potion"] = mana_potion;
	classification_map["Necromancer Shrunken Head"] = necromancer_shrunken_head;
	classification_map["Paladin Shield"] = paladin_shield;
	classification_map["Polearm"] = polearm;
	classification_map["Quest Item"] = quest_item;
	classification_map["Rejuvenation Potion"] = rejuvenation_potion;
	classification_map["Ring"] = ring;
	classification_map["Rune"] = rune;
	classification_map["Scepter"] = scepter;
	classification_map["Scroll"] = scroll;
	classification_map["Shield"] = shield;
	classification_map["Small Charm"] = small_charm;
	classification_map["Sorceress Orb"] = sorceress_orb;
	classification_map["Spear"] = spear;
	classification_map["Staff"] = staff;
	classification_map["Stamina Potion"] = stamina_potion;
	classification_map["Sword"] = sword;
	classification_map["Thawing Potion"] = thawing_potion;
	classification_map["Throwing Axe"] = throwing_axe;
	classification_map["Throwing Knife"] = throwing_knife;
	classification_map["Throwing Potion"] = throwing_potion;
	classification_map["Tome"] = tome;
	classification_map["Torch"] = torch;
	classification_map["Wand"] = wand;

	std::vector<std::string> lines;
	if(!nil::read_lines(file, lines))
		throw nil::exception("Unable to read item data");

	for(std::size_t i = 0; i != lines.size(); i++)
	{
		std::string const & line = lines[i];
		try
		{
			std::vector<std::string> tokens = nil::string::tokenise(line, "|");
			if(tokens.empty())
				continue;
			if(tokens.size() != 8)
			{
				std::cout << "Invalid token count: " << tokens.size() << std::endl;
				throw nil::exception("Unable to parse item data file due to invalid token count on a line");
			}
			std::string const & name = tokens[0];
			std::string const & code = tokens[1];
			std::string const & classification_string = tokens[2];
			ulong width = nil::string::string_to_number<ulong>(tokens[3]);
			ulong height = nil::string::string_to_number<ulong>(tokens[4]);
			bool stackable = (nil::string::string_to_number<ulong>(tokens[5]) != 0);
			bool useable = (nil::string::string_to_number<ulong>(tokens[6]) != 0);
			bool throwable = (nil::string::string_to_number<ulong>(tokens[7]) != 0);
			classification_map_type::iterator result = classification_map.find(classification_string);
			if(result == classification_map.end())
				throw nil::exception("Unable to parse item classification");
			item_classification_type classification = result->second;
			item_entry new_entry(name, code, classification, width, height, stackable, useable, throwable);
			items.push_back(new_entry);
		}
		catch(nil::exception & exception)
		{
			std::cout << "\"" << line << "\"" << std::endl;
			std::cout << "Invalid entry on line " << (i + 1) << ": " << exception.get_message() << std::endl;
			throw nil::exception("Item data error");
		}
	}

	std::cout << "Successfully loaded " << file << " (" << items.size() << " items)" << std::endl;
}

bool item_data_type::get(std::string const & code, item_entry & output)
{
	for(std::vector<item_entry>::iterator i = items.begin(), end = items.end(); i != end; i++)
	{
		item_entry & current_entry = *i;
		if(current_entry.type == code)
		{
			output = current_entry;
			return true;
		}
	}
	return false;
}

plain_text_data_type::plain_text_data_type()
{
}

plain_text_data_type::plain_text_data_type(std::string const & file)
{
	string_vector line_data;
	if(!nil::read_lines(file, line_data))
		throw nil::exception("Unable to open plain text data file");
	for(string_vector::iterator i = line_data.begin(), end = line_data.end(); i != end; i++)
	{
		string_vector tokens = nil::string::tokenise(*i, "|");
		lines.push_back(tokens);
	}
	std::cout << "Successfully loaded " << file << " (" << lines.size() << " lines)" << std::endl;
}

bool plain_text_data_type::get(std::size_t offset, std::string & output)
{
	if(offset < 0 || offset >= lines.size())
		return false;
	string_vector & line = lines[offset];
	if(line.empty())
		output = "";
	else
		output = line[0];
	return true;
}

bool plain_text_data_type::get(std::size_t offset, string_vector & output)
{
	if(offset < 0 || offset >= lines.size())
		return false;
	output = lines[offset];
	return true;
}

binary_data_type::binary_data_type()
{
}

binary_data_type::binary_data_type(std::string const & file)
{
	if(!nil::read_file(file, data))
		throw nil::exception("Unable to open binary data file");
	std::cout << "Successfully loaded " << file << " (" << data.size() << " bytes)" << std::endl;
}

bool binary_data_type::get(std::size_t offset, std::size_t length, std::string & output)
{
	if(offset < 0 || offset + length > data.size())
		return false;
	output = data.substr(offset, length);
	return true;
}

data_manager::data_manager()
{
}

data_manager::data_manager(std::string plain_text_data_directory)
{
	fix_directory(plain_text_data_directory);

	plain_text_data_type * plain_text_data_objects[] =
	{
		&magical_prefixes,
		&magical_suffixes,
		&rare_prefixes,
		&rare_suffixes,
		&unique_items,
		&monster_names,
		&monster_fields,
		&super_uniques,
		&item_properties,
		&skills
	};

	std::string const plain_text_data_file_names[] =
	{
		"magical_prefixes.txt",
		"magical_suffixes.txt",
		"rare_prefixes.txt",
		"rare_suffixes.txt",
		"unique_items.txt",
		"monster_names.txt",
		"monster_fields.txt",
		"super_uniques.txt",
		"item_properties.txt",
		"skills.txt"
	};

	/*
	binary_data_type * binary_data_objects[] =
	{
		&monster_stats
	};

	std::string const binary_data_file_names[] =
	{
		"monstats2.bin"
	};
	*/

	std::string item_data_file_name = plain_text_data_directory + "item_data.txt";
	std::cout << "Loading plain text item data file " << item_data_file_name << std::endl;
	item_data = item_data_type(item_data_file_name);

	for(std::size_t i = 0; i < nil::countof(plain_text_data_objects); i++)
	{
		plain_text_data_type & object = *plain_text_data_objects[i];
		std::string file_name = plain_text_data_directory + plain_text_data_file_names[i];
		std::cout << "Loading plain text data file " << file_name << std::endl;
		object = plain_text_data_type(file_name);
	}

	/*
	for(std::size_t i = 0; i < nil::countof(binary_data_objects); i++)
	{
		binary_data_type & object = *binary_data_objects[i];
		std::string file_name = binary_data_directory + binary_data_file_names[i];
		std::cout << "Loading binary data file " << file_name << std::endl;
		object = binary_data_type(file_name);
	}
	*/
}
