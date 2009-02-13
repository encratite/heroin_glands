#include <iostream>
#include <cstdlib>
#include <nil/file.hpp>
#include <nil/array.hpp>
#include <nil/string.hpp>
#include "excel_data.hpp"
#include "creation.hpp"
#include "utility.hpp"

void create_rare_affix_file(std::string const & rareaffix_txt, string_table_manager & string_manager, std::string const & output_directory, std::string const & output_file, std::string const & affix_description)
{
	std::stringstream rare_affix_stream;

	excel_data rareaffix_data(rareaffix_txt);
	std::string const rareaffix_column_strings[] = {"name"};
	string_vector rareaffix_columns = array_to_vector(rareaffix_column_strings, nil::countof(rareaffix_column_strings));
	string_vectors rareaffix_output;
	rareaffix_data.read_lines(rareaffix_columns, rareaffix_output);
	for(std::size_t i = 0; i < rareaffix_output.size(); i++)
	{
		string_vector line = rareaffix_output[i];
		std::string name = string_manager.transform(line[0]);

		rare_affix_stream << name << "\n";
	}

	write_file(affix_description, output_directory, output_file, rare_affix_stream);
}

void create_rare_affix_data(std::string const & rareprefix_txt, std::string const & raresuffix_txt, string_table_manager & string_manager, std::string const & output_directory)
{
	create_rare_affix_file(rareprefix_txt, string_manager, output_directory, "rare_prefixes.txt", "rare prefixes");
	create_rare_affix_file(raresuffix_txt, string_manager, output_directory, "rare_suffixes.txt", "rare suffixes");
}
