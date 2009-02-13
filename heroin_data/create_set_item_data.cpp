#include <iostream>
#include <cstdlib>
#include <nil/file.hpp>
#include <nil/array.hpp>
#include <nil/string.hpp>
#include "excel_data.hpp"
#include "creation.hpp"
#include "utility.hpp"

void create_set_item_data(std::string const & setitems_txt, string_table_manager & string_manager, std::string const & output_directory)
{
	std::stringstream setitems_stream;

	excel_data setitems_data(setitems_txt);
	string_vectors setitems_output;
	setitems_data.read_lines(setitems_output);
	for(std::size_t i = 0; i < setitems_output.size(); i++)
	{
		string_vector line = setitems_output[i];
		std::string const & name = line[0];
		std::string const & set = line[1];
		std::string const & code = line[2];
		std::string const & level_requirement = line[6];

		setitems_stream << string_manager.transform(name) << "|" << set << "|" << code << "|" << level_requirement;

		for(std::size_t j = 17; j <= 60; j += 4)
		{
			std::string const & prop_code = line[j];
			if(prop_code.empty())
				continue;
			std::string prop = mod_map[prop_code];
			if(prop.empty())
			{
				std::cout << "Failed to parse set item mod on " << name << ": " << prop_code << std::endl;
				return;
			}
			setitems_stream << "|" << prop << "," << line[j + 1] << "," << line[j + 2] << "," << line[j + 3];
		}

		setitems_stream << "\n";
	}

	write_file("set items data", output_directory, "set_items.txt", setitems_stream);
}
