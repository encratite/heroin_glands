#include <iostream>
#include <cstdlib>
#include <nil/file.hpp>
#include <nil/array.hpp>
#include <nil/string.hpp>
#include "excel_data.hpp"
#include "creation.hpp"
#include "utility.hpp"

void create_levels_data(std::string const & levels_txt, string_table_manager & string_manager, std::string const & output_directory)
{
	std::stringstream levels_stream;

	excel_data levels_data(levels_txt);
	std::string const levels_column_strings[] = {"LevelName"};
	string_vector levels_columns = array_to_vector(levels_column_strings, nil::countof(levels_column_strings));
	string_vectors levels_output;
	levels_data.read_lines(levels_columns, levels_output);
	for(std::size_t i = 0; i < levels_output.size(); i++)
	{
		string_vector line = levels_output[i];
		std::string name = string_manager.transform(line[0]);

		levels_stream << name << "\n";
	}

	write_file("level data", output_directory, "levels.txt", levels_stream);
}
