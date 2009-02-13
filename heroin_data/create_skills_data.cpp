#include <iostream>
#include <cstdlib>
#include <nil/file.hpp>
#include <nil/array.hpp>
#include <nil/string.hpp>
#include "excel_data.hpp"
#include "creation.hpp"
#include "utility.hpp"

void create_skills_data(std::string const & skills_txt, std::string const & output_directory)
{
	std::stringstream skills_stream, skills_enum_stream;

	excel_data skills_data(skills_txt);
	std::string const skills_column_strings[] = {"skill"};
	string_vector skills_columns = array_to_vector(skills_column_strings, nil::countof(skills_column_strings));
	string_vectors skills_output;
	skills_data.read_lines(skills_columns, skills_output);
	for(std::size_t i = 0; i < skills_output.size(); i++)
	{
		string_vector line = skills_output[i];
		std::string const & name = line[0];
		std::string enum_name = enumify(name);
		if(enum_name == "throw")
			enum_name = "throw_item";

		skills_stream << name << "\n";
		skills_enum_stream << enum_name << ",\n";
	}

	write_file("skills data", output_directory, "skills.txt", skills_stream);
	write_file("skills enum data", output_directory, "skills.hpp", skills_enum_stream);
}
