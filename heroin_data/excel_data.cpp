#include <iostream>
#include <nil/string.hpp>
#include "excel_data.hpp"

excel_data::excel_data(std::string const & input)
{
	lines = nil::string::tokenise(nil::string::erase(input, "\r"), "\n");
}

std::size_t vector_offset(std::string const & target, string_vector const & vector)
{
	for(std::size_t i = 0; i < vector.size(); i++)
	{
		if(vector[i] == target)
			return i;
	}
	return -1;
}

void excel_data::read_lines(string_vector const & targets, string_vectors & output)
{
	if(lines.empty())
		return;
	string_vector columns = nil::string::tokenise(lines[0], "\t");
	std::vector<std::size_t> column_indexes;
	for(std::size_t i = 0; i < targets.size(); i++)
	{
		std::string const & column = targets[i];
		std::size_t offset = vector_offset(column, columns);
		if(offset == -1)
		{
			std::cout << "Failed to locate column \"" << column << "\"" << std::endl;
			return;
		}
		//std::cout << "Found column \"" << column << "\" at offset " << offset << std::endl;
		column_indexes.push_back(offset);
	}
	for(std::size_t i = 1; i < lines.size(); i++)
	{
		std::string const & line = lines[i];
		if(line.empty() || line.find("Expansion") != std::string::npos)
			continue;
		string_vector element;
		string_vector tokens = nil::string::tokenise(line, "\t");
		for(std::size_t j = 0; j < column_indexes.size(); j++)
		{
			std::size_t column_index = column_indexes[j];
			element.push_back(tokens[column_index]);
		}
		output.push_back(element);
	}
}

void excel_data::read_lines(string_vectors & output)
{
	if(lines.empty())
		return;
	for(std::size_t i = 1; i < lines.size(); i++)
	{
		std::string const & line = lines[i];
		if(line.empty() || line.find("Expansion") != std::string::npos)
			continue;
		string_vector element;
		string_vector tokens = nil::string::tokenise(line, "\t");
		for(std::size_t j = 0; j < tokens.size(); j++)
			element.push_back(tokens[j]);
		output.push_back(element);
	}
}
