#include <iostream>
#include <nil/types.hpp>
#include "string_table.hpp"

ulong read_number(std::string const & input, std::size_t offset, std::size_t size)
{
	ulong output = 0;
	for(std::size_t i = 0; i < size; i++, offset++)
	{
		unsigned char byte = static_cast<unsigned char>(input[offset]);
		output |= byte << (i * 8);
	}
	return output;
}

bool read_string(std::string const & input, std::size_t & offset, std::string & output)
{
	std::size_t next_null = input.find('\x00', offset);
	if(next_null == std::string::npos)
		return false;
	output = input.substr(offset, next_null - offset);
	offset = next_null + 1;
	return true;
}

string_table::string_table(std::string const & data)
{
	if(data.size() < 12)
	{
		std::cout << "Unable to read size header from string table" << std::endl;
		return;
	}

	std::size_t offset = static_cast<std::size_t>(read_number(data, 9, 4));
	std::cout << "String table offset: " << std::hex << offset << std::endl;
	if(offset > data.size())
	{
		std::cout << "String offset too large for table" << std::endl;
		return;
	}

	while(true)
	{
		std::string key, value;
		if(!read_string(data, offset, key))
			break;
		if(!read_string(data, offset, value))
			break;
		entries[key] = value;
	}
}

bool string_table::transform(std::string & string)
{
	std::string replacement = entries[string];
	if(replacement.empty())
		return false;
	string = replacement;
	return true;
}
