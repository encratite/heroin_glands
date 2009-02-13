#include "check_revision.hpp"

#include <nil/string.hpp>
#include <nil/array.hpp>
#include <nil/file.hpp>

#include "utility.hpp"

//code largely copied from JBLS

namespace
{
	ulong const mpq_hash_codes[] =
	{
		0xE7F4CB62ul,
		0xF6A14FFCul,
		0xAA5504AFul,
		0x871FCDC2ul,
		0x11BF6A18ul,
		0xC57292E6ul,
		0x7927D27Eul,
		0x2FEC8733ul
	};

	std::string const d2_files[] =
	{
		"Game.exe",
		"Bnclient.dll",
		"D2Client.dll"
	};
}

ulong operator_add(ulong left, ulong right)
{
	return left + right;
}

ulong operator_sub(ulong left, ulong right)
{
	return left - right;
}

ulong operator_xor(ulong left, ulong right)
{
	return left ^ right;
}

bool get_variable_index(char input, std::size_t & offset)
{
	if(input >= 'A' && input <= 'C')
	{
		offset = static_cast<std::size_t>(input - 'A');
		return true;
	}
	return false;
}

bool get_mpq_index(std::string const & mpq, std::size_t & offset)
{
	if(mpq.length() != 14)
		return false;
	
	char input = mpq[9];

	if(input >= '0' && input <= '9')
	{
		offset = static_cast<std::size_t>(input - '0');
		return true;
	}
	return false;
}

check_revision_result_type check_revision(std::string const & formula, std::string const & mpq, std::string const & directory, ulong & output)
{
	/*
	Examples:
	std::string const formula = "A=4095648652 B=3744856545 C=4182215876 4 A=A^S B=B+C C=C+A A=A^B";
	std::string const mpq = "ver-IX86-4.mpq";
	*/

	typedef ulong (*operator_type)(ulong, ulong);


	//thanks to Zoxc for noticing the operator count bug
	std::size_t const variable_count = 3;
	std::size_t const operator_count = 4;

	ulong values[variable_count];
	operator_type operators[operator_count];

	std::vector<std::string> tokens = nil::string::tokenise(formula, " ");

	std::size_t offset;
	for(offset = 0; offset < tokens.size(); offset++)
	{
		std::string const & token = tokens[offset];
		if(token == "4")
		{
			offset++;
			break;
		}
		else if(token.size() < 3)
			return check_revision_result_formula_error;
		char variable_letter = token[0];
		std::string number_string = token.substr(2);
		ulong number;
		if(!nil::string::string_to_number<ulong>(number_string, number))
			return check_revision_result_formula_error;
		std::size_t variable_index;
		if(!get_variable_index(variable_letter, variable_index))
			return check_revision_result_formula_error;
		values[variable_index] = number;
	}

	for(std::size_t i = 0; offset < tokens.size(); i++, offset++)
	{
		std::string const & token = tokens[offset];
		if(token.size() != 5)
			return check_revision_result_formula_error;
		operator_type current_operator;
		switch(token[3])
		{
			case '+':
				current_operator = &operator_add;
				break;
			case '-':
				current_operator = &operator_sub;
				break;
			case '^':
				current_operator = &operator_xor;
				break;
			default:
				return check_revision_result_formula_error;
		}
		operators[i] = current_operator;
	}

	std::size_t mpq_index;
	if(!get_mpq_index(mpq, mpq_index))
		return check_revision_result_mpq_error;

	ulong mpq_hash = mpq_hash_codes[mpq_index];

	ulong a = values[0];
	ulong b = values[1];
	ulong c = values[2];

	a ^= mpq_hash;

	for(std::size_t i = 0; i < nil::countof(d2_files); i++)
	{
		std::string content;
		std::string file = directory + d2_files[i];
		if(!nil::read_file(file, content))
			return check_revision_result_file_error;
		for(std::size_t j = 0; j < content.length(); j += 4)
		{
			ulong s = read_dword(content, j);
			//A=A^S B=B+C C=C+A A=A^B
			//the variable positions are fixed
			a = operators[0](a, s);
			b = operators[1](b, c);
			c = operators[2](c, a);
			a = operators[3](a, b);
		}
	}

	output = c;

	return check_revision_result_success;
}
