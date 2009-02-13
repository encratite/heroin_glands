#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "types.hpp"

void initialise_mod_map();
void initialise_stat_cost_map();

string_vector array_to_vector(std::string const * array, std::size_t size);
std::string enumify(std::string const & input);
bool write_file(std::string const & description, std::string const & output_directory, std::string const & file_name, std::stringstream & data);
void zeroify(std::string & input);

extern string_map mod_map, stat_cost_map;

#endif
