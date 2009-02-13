#include <iostream>
#include <cstdlib>
#include <nil/file.hpp>
#include <nil/array.hpp>
#include <nil/string.hpp>
#include "excel_data.hpp"
#include "creation.hpp"
#include "utility.hpp"

void create_monster_data(std::string const & monstats_txt, std::string const & monstats2_bin, string_table_manager & string_manager, std::string const & output_directory)
{
	std::stringstream monstats_stream;

	excel_data monstats_data(monstats_txt);
	std::string const monstats_column_strings[] = {"NameStr"};
	string_vector monstats_columns = array_to_vector(monstats_column_strings, nil::countof(monstats_column_strings));
	string_vectors monstats_output;
	monstats_data.read_lines(monstats_columns, monstats_output);
	for(std::size_t i = 0; i < monstats_output.size(); i++)
	{
		string_vector line = monstats_output[i];
		std::string name = line[0];
		name = string_manager.transform(name);
		if(name == "an evil force")
		{
			name = "";
			//continue;
		}

		monstats_stream << name << "\n";
	}

	write_file("monster names", output_directory, "monster_names.txt", monstats_stream);

	std::stringstream monstats2_stream;
	for(std::size_t i = 4; i < monstats2_bin.size(); i += 0x134)
	{
		bool first = true;
		for(std::size_t j = 0; j < 0x10; j++)
		{
			if(first)
				first = false;
			else
				monstats2_stream << "|";
			monstats2_stream << (ulong)(unsigned char)monstats2_bin[i + 0x15 + j];
		}
		monstats2_stream << "\n";
	}

	write_file("monster field lengths", output_directory, "monster_fields.txt", monstats2_stream);
}
