#include <iostream>
#include <nil/string.hpp>
#include <nil/array.hpp>
#include <nil/file.hpp>
#include "utility.hpp"

string_vector array_to_vector(std::string const * array, std::size_t size)
{
	string_vector output;
	for(std::size_t i = 0; i < size; i++)
		output.push_back(array[i]);
	return output;
}

std::string enumify(std::string const & input)
{
	std::string const targets[] =
	{
		"/",
		"(",
		")",
		",",
		".",
		"'"
	};
	std::string output = nil::string::replace(nil::string::to_lower(input), " ", "_");
	for(std::size_t i = 0; i < nil::countof(targets); i++)
		output = nil::string::erase(output, targets[i]);
	return output;
}

bool write_file(std::string const & description, std::string const & output_directory, std::string const & file_name, std::stringstream & data)
{
	std::string path = output_directory + file_name;

	if(nil::write_file(path, data.str()))
	{
		std::cout << "Successfully wrote " << description << " to " << path << std::endl;
		return true;
	}
	else
	{
		std::cout << "Failed to write " << description << " to " << path << std::endl;
		return false;
	}
}

void zeroify(std::string & input)
{
	if(input.empty())
		input = "0";
}
