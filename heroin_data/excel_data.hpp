#ifndef EXCEL_DATA_HPP
#define EXCEL_DATA_HPP

#include <string>
#include <vector>
#include "types.hpp"

class excel_data
{
public:
	excel_data(std::string const & input);
	void read_lines(string_vector const & targets, string_vectors & output);
	void read_lines(string_vectors & output);

private:
	string_vector lines;
};

#endif
