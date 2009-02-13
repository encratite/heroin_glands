#ifndef STRING_TABLE_MANAGER
#define STRING_TABLE_MANAGER

#include "string_table.hpp"

class string_table_manager
{
public:
	string_table_manager(std::string const & patchstring_tbl, std::string const & expansionstring_tbl, std::string const & string_tbl);
	std::string transform(std::string const & input);

	bool debug;

private:
	std::vector<string_table> string_tables;
};

#endif
