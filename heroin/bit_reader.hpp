#ifndef BIT_READER_HPP
#define BIT_READER_HPP

#include <string>

#include <nil/types.hpp>

class bit_reader
{
public:
	bit_reader(std::string const & data);
	ulong read(std::size_t length);
	bool read_bool();
	void set_offset(std::size_t new_offset);
	std::string string();

	bool debugging;

private:
	std::string data;
	std::size_t offset;
};

#endif
