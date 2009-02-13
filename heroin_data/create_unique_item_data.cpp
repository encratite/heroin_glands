#include <iostream>
#include <cstdlib>
#include <nil/file.hpp>
#include <nil/array.hpp>
#include <nil/string.hpp>
#include "excel_data.hpp"
#include "creation.hpp"
#include "utility.hpp"

void create_unique_item_data(std::string const & uniqueitems_txt, string_table_manager & string_manager, std::string const & output_directory)
{
	std::stringstream uniqueitems_stream;

	excel_data uniqueitems_data(uniqueitems_txt);
	string_vectors uniqueitems_output;
	uniqueitems_data.read_lines(uniqueitems_output);
	for(std::size_t i = 0; i < uniqueitems_output.size(); i++)
	{
		string_vector line = uniqueitems_output[i];
		std::string const & name = line[0];
		std::string const & level_requirement = line[7];
		std::string const & code = line[8];

		//there are unused entries and separators inside the file, blank them
		if(code.empty())
		{
			uniqueitems_stream << "\n";
			continue;
		}

		uniqueitems_stream << string_manager.transform(name) << "|" << code << "|" << level_requirement;

		for(std::size_t j = 21; j <= 68; j += 4)
		{
			std::string const & prop_code = line[j];
			if(prop_code.empty())
				continue;

			//not sure about these, skipping them for now until proven otherwise
			if(prop_code[0] == '*')
				continue;

			std::string prop = mod_map[prop_code];
			if(prop.empty())
			{
				std::cout << "Failed to parse set item mod on " << name << ": " << prop_code << std::endl;
				return;
			}
			uniqueitems_stream << "|" << prop << "," << line[j + 1] << "," << line[j + 2] << "," << line[j + 3];
		}

		uniqueitems_stream << "\n";
	}

	write_file("unique item data", output_directory, "unique_items.txt", uniqueitems_stream);
}
