#include <iostream>
#include <cstdlib>
#include <nil/file.hpp>
#include <nil/array.hpp>
#include <nil/string.hpp>
#include "excel_data.hpp"
#include "creation.hpp"
#include "utility.hpp"

void create_affixes(std::string const & affix_data, std::string const & affix_descriptor, std::string const & output_file, string_table_manager & string_manager, std::string const & output_directory)
{
	std::stringstream magical_affixes_stream;

	excel_data magicaffix_data(affix_data);
	std::string const magicaffix_column_strings[] = {"Name", "levelreq", "mod1code", "mod1param", "mod1min", "mod1max", "mod2code", "mod2param", "mod2min", "mod2max", "mod3code", "mod3param", "mod3min", "mod3max"};
	string_vector magicaffix_columns = array_to_vector(magicaffix_column_strings, nil::countof(magicaffix_column_strings));
	string_vectors magicaffix_output;
	magicaffix_data.read_lines(magicaffix_columns, magicaffix_output);
	for(std::size_t i = 0; i < magicaffix_output.size(); i++)
	{
		//wow this code is terrible
		string_vector line = magicaffix_output[i];
		std::size_t offset = 0;
		std::string name = string_manager.transform(line[offset++]);
		//if(name.empty())
		//	magical_affixes_stream << "\n";

		std::string const & level_requirement = line[offset++];

		std::string const & mod1code = line[offset++];
		std::string const & mod1param = line[offset++];
		std::string const & mod1min = line[offset++];
		std::string const & mod1max = line[offset++];

		std::string const & mod2code = line[offset++];
		std::string const & mod2param = line[offset++];
		std::string const & mod2min = line[offset++];
		std::string const & mod2max = line[offset++];

		std::string const & mod3code = line[offset++];
		std::string const & mod3param = line[offset++];
		std::string const & mod3min = line[offset++];
		std::string const & mod3max = line[offset++];

		std::string mod1 = mod_map[mod1code];
		std::string mod2 = mod_map[mod2code];
		std::string mod3 = mod_map[mod3code];

		if(
			(!mod1code.empty() && mod1.empty()) ||
			(!mod2code.empty() && mod2.empty()) ||
			(!mod3code.empty() && mod3.empty())
		)
		{
			std::cout << "Failed to parse mod: " << name << ": " << mod1code << ", " << mod2code << ", " << mod3code << std::endl;
			std::cout << "Parsed: " << mod1 << ", " << mod2 << ", " << mod3 << std::endl;
			return;
		}

		std::string mod1string;
		if(!mod1code.empty())
			mod1string = mod1 + "," + mod1param + "," + mod1min + "," + mod1max;

		std::string mod2string;
		if(!mod2code.empty())
			mod2string = mod2 + "," + mod2param + "," + mod2min + "," + mod2max;

		std::string mod3string;
		if(!mod3code.empty())
			mod3string = mod3 + "," + mod3param + "," + mod3min + "," + mod3max;

		magical_affixes_stream << name << "|" << level_requirement << "|" << mod1string << "|" << mod2string << "|" << mod3string << "\n";
	}

	write_file("magical " + affix_descriptor, output_directory, output_file, magical_affixes_stream);
}

void create_magical_affix_data(std::string const & magicprefix_txt, std::string const & magicsuffix_txt, string_table_manager & string_manager, std::string const & output_directory)
{
	create_affixes(magicprefix_txt, "prefixes", "magical_prefixes.txt", string_manager, output_directory);
	create_affixes(magicsuffix_txt, "suffixes", "magical_suffixes.txt", string_manager, output_directory);
}
