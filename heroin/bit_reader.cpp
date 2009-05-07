#include <iostream>

#include <heroin/bit_reader.hpp>
#include <heroin/utility.hpp>

#include <nil/memory.hpp>
#include <nil/exception.hpp>

bit_reader::bit_reader(std::string const & data):
	offset(0),
	data(data),
	debugging(false)
{
}

ulong bit_reader::read(std::size_t length)
{
	if(debugging)
		std::cout << "Reading " << length << " bit(s)" << std::endl;
	ulong output = nil::read_little_endian(data.c_str(), offset, length);
	std::size_t new_offset = offset + length;
	set_offset(new_offset);
	return output;
}

bool bit_reader::read_bool()
{
	return read(1) != 0;
}

std::string bit_reader::string()
{
	std::string output;
	for(std::size_t i = 0; i < 16; i++)
	{
		char letter = static_cast<char>(read(7));
		if(letter == 0)
			return output;
		output.push_back(letter);
	}
	return output;
}

void bit_reader::set_offset(std::size_t new_offset)
{
	std::size_t maximum = data.size() * 8;
	if(new_offset > maximum)
	{
		offset = maximum;
		throw nil::exception("bit reader error, bit out of range");
	}
	else
		offset = new_offset;
}
