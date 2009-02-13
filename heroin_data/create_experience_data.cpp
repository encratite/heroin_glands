#include <iostream>
#include <cstdlib>
#include <nil/file.hpp>
#include <nil/array.hpp>
#include <nil/string.hpp>
#include "excel_data.hpp"
#include "creation.hpp"
#include "utility.hpp"

void create_experience_data(std::string const & experience_txt, std::string const & output_directory)
{
	std::stringstream experience_stream;

	excel_data experience_data(experience_txt);
	std::string const experience_column_strings[] = {"Amazon", "ExpRatio"};
	string_vector experience_columns = array_to_vector(experience_column_strings, nil::countof(experience_column_strings));
	string_vectors experience_output;
	experience_data.read_lines(experience_columns, experience_output);
	for(std::size_t i = 2; i < experience_output.size(); i++)
	{
		string_vector line = experience_output[i];
		std::string const & experience = line[0];
		std::string const & ratio = line[1];

		experience_stream << experience << "|" << ratio << "\n";
	}

	write_file("experience data", output_directory, "experience.txt", experience_stream);
}
