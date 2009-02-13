#include <iostream>
#include <cstdlib>
#include <nil/file.hpp>
#include <nil/array.hpp>
#include <nil/string.hpp>
#include "excel_data.hpp"
#include "creation.hpp"
#include "utility.hpp"

void create_super_uniques_data(std::string const & superuniques_txt, string_table_manager & string_manager, std::string const & output_directory)
{
	std::stringstream superuniques_stream;

	excel_data superuniques_data(superuniques_txt);
	std::string const superuniques_column_strings[] = {"Superunique"};
	string_vector superuniques_columns = array_to_vector(superuniques_column_strings, nil::countof(superuniques_column_strings));
	string_vectors superuniques_output;
	superuniques_data.read_lines(superuniques_columns, superuniques_output);
	for(std::size_t i = 0; i < superuniques_output.size(); i++)
	{
		string_vector line = superuniques_output[i];
		std::string name = string_manager.transform(line[0]);

		superuniques_stream << name << "\n";
	}

	write_file("super unique monster data", output_directory, "super_uniques.txt", superuniques_stream);
}
