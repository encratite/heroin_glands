#ifndef STRING_TABLE_HPP
#define STRING_TABLE_HPP

#include <string>
#include "types.hpp"

class string_table
{
public:
	string_table(std::string const & data);
	bool transform(std::string & string);

private:
	string_map entries;
};

#endif
