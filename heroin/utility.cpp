#include <heroin/utility.hpp>

#include <iostream>
#include <sstream>

#include <nil/time.hpp>
#include <nil/random.hpp>
#include <nil/environment.hpp>

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

std::string get_char_array_string(std::string const & data)
{
	std::stringstream stream;
	bool first = true;
	stream << "unsigned char const bytes[] = {";
	for(std::size_t i = 0; i < data.size(); i++)
	{
		if(first)
			first = false;
		else
			stream << ", ";
		stream << "0x";
		stream << std::hex;
		stream.width(2);
		stream.fill('0');
		stream << get_byte(data, i);
	}
	stream << "};";
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

std::string read_string(std::string const & packet, std::size_t & offset)
{
	std::size_t zero = packet.find('\x00', offset);
	std::string output;
	if(zero == std::string::npos)
	{
		zero = packet.length();
		output = packet.substr(offset, zero - offset);
		offset = zero;
	}
	else
	{
		output = packet.substr(offset, zero - offset);
		offset = zero + 1;
	}
	return output;
}

std::string read_one_string(std::string const & packet, std::size_t offset)
{
	return read_string(packet, offset);
}

std::string get_character_class_string(ulong class_identifier)
{
	switch(class_identifier)
	{
		case 1: return "Amazon";
		case 2: return "Sorceress";
		case 3: return "Necromancer";
		case 4: return "Paladin";
		case 5: return "Barbarian";
		case 6: return "Druid";
		case 7: return "Assassin";
		default: return "Unknown";
	}
}

char get_upper_case_letter()
{
	return static_cast<char>('A' + nil::random::uint(0, 25));
}

char get_lower_case_letter()
{
	return static_cast<char>('a' + nil::random::uint(0, 25));
}

char get_digit()
{
	return static_cast<char>('0' + nil::random::uint(0, 9));
}

char get_letter(bool first)
{
	if(nil::random::uint(1, 36) <= 10)
		return get_digit();
	else
		return first ? get_upper_case_letter() : get_lower_case_letter();
}

std::string generate_string()
{
	std::string output;
	for(long i = 0; i < 15; i++)
		output += get_letter(i == 0);
	return output;
}

std::string construct_bncs_packet(ulong command, std::string const & arguments)
{
	std::string packet = "\xff" + byte_to_string(command) + word_to_string(4 + arguments.length()) + arguments;
	return packet;
}

std::string construct_mcp_packet(ulong command, std::string const & arguments)
{
	std::string packet = word_to_string(3 + arguments.length()) + byte_to_string(command) + arguments;
	return packet;
}

void fix_directory(std::string & directory)
{
	if(!directory.empty())
	{
		char last_byte = *(directory.end() - 1);
#ifdef NIL_WINDOWS
		char separator = '\\';
#else
		char separator = '/';
#endif
		if(last_byte != separator)
			directory += separator;
	}
}
