#include "utility.hpp"

#include <iostream>
#include <sstream>

#include <nil/time.hpp>

std::string get_data_string(std::string const & data)
{
	std::stringstream stream;
	for(std::size_t i = 0; i < data.size(); i++)
	{
		stream << std::hex;
		stream.width(2);
		stream.fill('0');
		stream << get_byte(data, i) << " ";
	}
	return stream.str();
}

void print_data(std::string const & data)
{
	std::cout << get_data_string(data) << std::endl;
}

void print_dword(ulong dword)
{
	std::cout << std::hex;
	std::cout.width(8);
	std::cout.fill('0');
	std::cout << dword;
}

std::string get_dword_string(ulong dword)
{
	std::stringstream stream;
	for(ulong i = 0; i < 4; i++)
	{
		stream << std::hex;
		stream.width(2);
		stream.fill('0');
		stream << ((dword >> (i * 8)) & 0xff);
	}
	return stream.str();
}

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

ulong read_nbo_number(std::string const & input, std::size_t offset, std::size_t size)
{
	ulong output = 0;
	for(std::size_t i = 0; i < size; i++, offset++)
	{
		unsigned char byte = static_cast<unsigned char>(input[offset]);
		output <<= 8;
		output |= byte;
	}
	return output;
}

ulong read_nbo_dword(std::string const & input, std::size_t offset)
{
	return read_nbo_number(input, offset, 4);
}

ulong read_nbo_word(std::string const & input, std::size_t offset)
{
	return read_nbo_number(input, offset, 2);
}

ulong read_dword(std::string const & input, std::size_t offset)
{
	return read_number(input, offset, 4);
}

ulong read_word(std::string const & input, std::size_t offset)
{
	return read_number(input, offset, 2);
}

ulong char_to_byte(char input)
{
	return static_cast<ulong>(static_cast<unsigned char>(input));
}

ulong read_byte(std::string const & input, std::size_t offset)
{
	return char_to_byte(input[offset]);
}

std::string number_to_string(ulong input, ulong size)
{
	std::string output;
	for(ulong i = 0; i < size; i++)
		output.push_back(static_cast<char>((input >> (i * 8)) & 0xff));
	return output;
}

std::string dword_to_string(ulong input)
{
	return number_to_string(input, 4);
}

std::string word_to_string(ulong input)
{
	return number_to_string(input, 2);
}

std::string byte_to_string(ulong input)
{
	return number_to_string(input, 1);
}

std::string char_to_string(char input)
{
	std::stringstream stream;
	stream << std::hex;
	stream.width(2);
	stream.fill('0');
	stream << char_to_byte(input);
	return stream.str();
}

ulong get_byte(std::string const & input, std::size_t offset)
{
	return static_cast<ulong>(static_cast<unsigned char>(input[offset]));
}

ulong get_tick_count()
{
	return static_cast<ulong>(nil::boot_time());
}
