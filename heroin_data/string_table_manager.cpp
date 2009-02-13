#include <iostream>
#include "string_table_manager.hpp"

string_table_manager::string_table_manager(std::string const & patchstring_tbl, std::string const & expansionstring_tbl, std::string const & string_tbl):
	debug(false)
{
	string_table
		patchstring_table(patchstring_tbl),
		expansionstring_table(expansionstring_tbl),
		string_table(string_tbl);

	string_tables.push_back(patchstring_table);
	string_tables.push_back(expansionstring_table);
	string_tables.push_back(string_table);
}

std::string string_table_manager::transform(std::string const & input)
{
	std::string output(input);
	for(std::size_t i = 0; i < string_tables.size(); i++)
	{
		string_table & table = string_tables[i];
		if(table.transform(output))
		{
			if(debug)
				std::cout << "String table transformation: \"" << input << "\" -> \"" << output << "\"" << std::endl;
			break;
		}
	}
	return output;
}
