#include <iostream>
#include <cstdlib>
#include <set>
#include <nil/file.hpp>
#include <nil/array.hpp>
#include <nil/string.hpp>
#include "excel_data.hpp"
#include "creation.hpp"
#include "utility.hpp"

void create_item_data(std::string const & armor_txt, std::string const & misc_txt, std::string const & weapons_txt, string_table_manager & string_manager, std::string const & output_directory)
{
	string_map type_map;
	type_map["helm"] = "Helm";
	type_map["tors"] = "Armor";
	type_map["shie"] = "Shield";
	type_map["glov"] = "Gloves";
	type_map["boot"] = "Boots";
	type_map["belt"] = "Belt";
	type_map["pelt"] = "Druid Pelt";
	type_map["phlm"] = "Barbarian Helm";
	type_map["ashd"] = "Paladin Shield";
	type_map["circ"] = "Circlet";
	type_map["head"] = "Necromancer Shrunken Head";

	type_map["elix"] = "Elixir";
	type_map["hpot"] = "Health Potion";
	type_map["mpot"] = "Mana Potion";
	type_map["spot"] = "Stamina Potion";
	type_map["apot"] = "Antidote Potion";
	type_map["rpot"] = "Rejuvenation Potion";
	type_map["wpot"] = "Thawing Potion";
	type_map["book"] = "Tome";
	type_map["amul"] = "Amulet";
	type_map["ring"] = "Ring";
	type_map["gold"] = "Gold";
	type_map["ques"] = "Quest Item";
	type_map["bowq"] = "Arrows";
	type_map["torc"] = "Torch";
	type_map["xboq"] = "Bolts";
	type_map["scro"] = "Scroll";
	type_map["body"] = "Body Part";
	type_map["key"] = "Key";
	type_map["play"] = "Ear";
	type_map["gema"] = "Gem";
	type_map["gemt"] = "Gem";
	type_map["gems"] = "Gem";
	type_map["geme"] = "Gem";
	type_map["gemr"] = "Gem";
	type_map["gemd"] = "Gem";
	type_map["gemz"] = "Gem";
	type_map["herb"] = "Herb";
	type_map["scha"] = "Small Charm";
	type_map["mcha"] = "Large Charm";
	type_map["lcha"] = "Grand Charm";
	type_map["rune"] = "Rune";
	type_map["jewl"] = "Jord";
	type_map["knif"] = "Dagger";
	type_map["tkni"] = "Throwing Knife";
	type_map["taxe"] = "Throwing Axe";
	type_map["jave"] = "Javelin";
	type_map["spea"] = "Spear";
	type_map["pole"] = "Polearm";
	type_map["staf"] = "Staff";
	type_map["bow"] = "Bow";
	type_map["xbow"] = "Crossbow";
	type_map["tpot"] = "Throwing Potion";
	type_map["h2h"] = "Assassin Katar";
	type_map["h2h2"] = "Assassin Katar";
	type_map["abow"] = "Amazon Bow";
	type_map["aspe"] = "Amazon Spear";
	type_map["orb"] = "Sorceress Orb";
	type_map["ajav"] = "Amazon Javelin";
	type_map["jewl"] = "Jewel";

	type_map["axe"] = "Axe";
	type_map["wand"] = "Wand";
	type_map["club"] = "Club";
	type_map["scep"] = "Scepter";
	type_map["mace"] = "Mace";
	type_map["hamm"] = "Hammer";
	type_map["swor"] = "Sword";
	type_map["knif"] = "Dagger";
	type_map["tkni"] = "Throwing Knife";
	type_map["taxe"] = "Throwing Axe";
	type_map["jave"] = "Javelin";
	type_map["spea"] = "Spear";
	type_map["pole"] = "Polearm";
	type_map["staf"] = "Staff";
	type_map["bow"] = "Bow";
	type_map["xbow"] = "Crossbow";
	type_map["tpot"] = "Throwing Potion";
	type_map["h2h"] = "Assassin Katar";
	type_map["h2h2"] = "Assassin Katar";
	type_map["abow"] = "Amazon Bow";
	type_map["aspe"] = "Amazon Spear";
	type_map["orb"] = "Sorceress Orb";
	type_map["ajav"] = "Amazon Javelin";

	std::stringstream item_stream;

	excel_data armor_data(armor_txt);
	std::string const armor_column_strings[] = {"code", "type", "invwidth", "invheight", "stackable", "useable", "throwable"};
	string_vector armor_columns = array_to_vector(armor_column_strings, nil::countof(armor_column_strings));
	string_vectors armor_output;
	armor_data.read_lines(armor_columns, armor_output);
	for(std::size_t i = 0; i < armor_output.size(); i++)
	{
		string_vector line = armor_output[i];
		std::string const & code = line[0];
		std::string type = line[1];
		std::string const & width = line[2];
		std::string const & height = line[3];
		std::string const & stackable = line[4];
		std::string const & useable = line[5];
		std::string const & throwable = line[6];

		std::string const & new_type = type_map[type];

		type = new_type;
		std::string name = string_manager.transform(code);

		item_stream << name << "|" << code << "|" << type << "|" << width << "|" << height << "|" << stackable << "|" << useable << "|" << throwable << "\n";
	}

	/*
	excel_data gems_data(gems_txt);
	std::string const gems_column_strings[] = {"name", "code"};
	string_vector gems_columns = array_to_vector(gems_column_strings, nil::countof(gems_column_strings));
	string_vectors gems_output;
	gems_data.read_lines(gems_columns, gems_output);
	for(std::size_t i = 0; i < gems_output.size(); i++)
	{
		string_vector line = gems_output[i];
		std::string const & name = line[0];
		std::string const & code = line[1];

		item_stream << name << "|" << code << "|" << (code[0] == 'r' ? "Rune" : "Gem") << "|1|1\n";
	}
	*/

	excel_data misc_data(misc_txt);
	std::string const misc_column_strings[] = {"code", "type", "invwidth", "invheight", "stackable", "useable", "throwable"};
	string_vector misc_columns = array_to_vector(misc_column_strings, nil::countof(misc_column_strings));
	string_vectors misc_output;
	misc_data.read_lines(misc_columns, misc_output);
	for(std::size_t i = 0; i < misc_output.size(); i++)
	{
		string_vector line = misc_output[i];
		std::string const & code = line[0];
		std::string type = line[1];
		std::string const & width = line[2];
		std::string const & height = line[3];
		std::string const & stackable = line[4];
		std::string const & useable = line[5];
		std::string const & throwable = line[6];

		std::string const & new_type = type_map[type];

		type = new_type;
		std::string name = string_manager.transform(code);

		std::size_t offset = name.rfind("\n");
		if(offset != std::string::npos)
		{
			std::string new_name = name.substr(offset + 1);
			std::cout << "Multi line hack: " << nil::string::replace(name, "\n", "\\n") << " -> " << new_name << std::endl;
			name = new_name;
		}

		item_stream << name << "|" << code << "|" << type << "|" << width << "|" << height << "|" << stackable << "|" << useable << "|" << throwable << "\n";
	}

	excel_data weapons_data(weapons_txt);
	std::string const weapons_column_strings[] = {"code", "type", "invwidth", "invheight", "stackable", "useable", "special"};
	string_vector weapons_columns = array_to_vector(weapons_column_strings, nil::countof(weapons_column_strings));
	string_vectors weapons_output;
	weapons_data.read_lines(weapons_columns, weapons_output);
	for(std::size_t i = 0; i < weapons_output.size(); i++)
	{
		string_vector line = weapons_output[i];
		std::string const & code = line[0];
		std::string type = line[1];
		std::string const & width = line[2];
		std::string const & height = line[3];
		std::string stackable = line[4];
		std::string const & useable = line[5];
		std::string const & special = line[6];
		std::string throwable;

		if(stackable.empty())
			stackable = "0";

		if(special == "primarily thrown")
			throwable = "1";
		else
			throwable = "0";

		std::string const & new_type = type_map[type];

		type = new_type;
		std::string name = string_manager.transform(code);

		item_stream << name << "|" << code << "|" << type << "|" << width << "|" << height << "|" << stackable << "|" << useable << "|" << throwable << "\n";
	}

	write_file("item data", output_directory, "item_data.txt", item_stream);

	std::set<std::string> item_class_set;
	for(string_map::iterator i = type_map.begin(), end = type_map.end(); i != end; i++)
		item_class_set.insert(i->second);

	std::stringstream item_classification_map_stream;
	for(std::set<std::string>::iterator i = item_class_set.begin(); i != item_class_set.end(); i++)
	{
		std::string const & item_class = *i;
		item_classification_map_stream << "classification_map[\"" << item_class << "\"] = " << enumify(item_class) << ";\n";
	}

	std::stringstream item_classification_stream;
	for(std::set<std::string>::iterator i = item_class_set.begin(); i != item_class_set.end(); i++)
		item_classification_stream << enumify(*i) << ",\n";

	write_file("item classification enum data", output_directory, "item_classification.hpp", item_classification_stream);
	write_file("item classification enum map data", output_directory, "item_classification.cpp", item_classification_map_stream);
}
