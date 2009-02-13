#include <iostream>
#include <cstdlib>
#include <nil/file.hpp>
#include <nil/array.hpp>
#include <nil/string.hpp>
#include "excel_data.hpp"
#include "creation.hpp"
#include "utility.hpp"

void create_item_property_data(std::string const & properties_txt, std::string const & itemstatcost_txt, std::string const & output_directory)
{
	/*
	std::stringstream item_property_enum, item_property_map;

	excel_data properties_data(properties_txt);
	std::string const properties_column_strings[] = {"code"};
	string_vector properties_columns = array_to_vector(properties_column_strings, nil::countof(properties_column_strings));
	string_vectors properties_output;
	properties_data.read_lines(properties_columns, properties_output);
	for(std::size_t i = 0; i < properties_output.size(); i++)
	{
		string_vector line = properties_output[i];
		std::string property = mod_map[line[0]];
		std::string enum_string;
		if(property.empty())
			enum_string = "unused_property";
		else if(property == "Throw")
			enum_string = "throw_item";
		else
			enum_string = enumify(property);

		item_property_enum << enum_string << ",\n";
		item_property_map << "item_properties[" << enum_string << "] = \"" << property << "\";\n";
	}

	//this is the point where I realised that this code is unnecessary - what a downer

	std::string item_property_enum_file = output_directory + "item_properties.hpp";
	std::cout << "Writing item properties enum data to " << item_property_enum_file << std::endl;
	nil::write_file(item_property_enum_file, item_property_enum.str());

	std::string item_property_map_file = output_directory + "item_properties.cpp";
	std::cout << "Writing item properties enum map data to " << item_property_map_file << std::endl;
	nil::write_file(item_property_map_file, item_property_map.str());
	*/

	std::stringstream itemstat_stream, itemstat_map_stream;

	excel_data itemstatcost_data(itemstatcost_txt);
	std::string const itemstatcost_column_strings[] = {"Stat", "Save Bits", "Save Param Bits", "Signed", "Save Add"};
	string_vector itemstatcost_columns = array_to_vector(itemstatcost_column_strings, nil::countof(itemstatcost_column_strings));
	string_vectors itemstatcost_output;
	itemstatcost_data.read_lines(itemstatcost_columns, itemstatcost_output);
	for(std::size_t i = 0; i < itemstatcost_output.size(); i++)
	{
		string_vector line = itemstatcost_output[i];
		std::string const & original_data = line[0];
		std::string save_bits = line[1];
		std::string save_param_bits = line[2];
		std::string signed_entry = line[3];
		std::string save_add = line[4];

		std::string stat = stat_cost_map[original_data];
		if(stat.empty())
		{
			std::cout << "Failed to parse \"" << original_data << "\", no such entry in stat cost map" << std::endl;
			return;
		}

		zeroify(save_bits);
		zeroify(save_param_bits);
		zeroify(signed_entry);
		zeroify(save_add);

		itemstat_stream << enumify(stat) << ",\n";
		//itemstat_map_stream << "property_map[" << enumify(stat) << "] = \"" << stat << "\";\n";
		//itemstat_map_stream << stat << "|" << save_bits << "|" << save_param_bits << "|" << signed_entry << "|" << save_add << "\n";
		itemstat_map_stream << stat << "|" << save_bits << "|" << save_param_bits << "|" << save_add << "\n";
	}

	write_file("item properties enum data", output_directory, "item_properties.hpp", itemstat_stream);
	write_file("item properties enum map data", output_directory, "item_properties.txt", itemstat_map_stream);
}
