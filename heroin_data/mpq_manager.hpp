#ifndef MPQ_MANAGER_HPP
#define MPQ_MANAGER_HPP

#include <string>
#include <vector>
#include "mpq.hpp"

class mpq_manager
{
public:
	mpq_manager(std::string const & directory);
	bool read(std::string const & file_name, std::string & output);
	bool read(std::string const & mpq_name, std::string const & file_name, std::string & output);
	bool extract_files(std::string const & file_name, std::string const & output_directory);

private:
	std::vector<mpq_data> mpq_files;
	std::vector<std::string> mpq_archive_names;

	void add_mpq_file(std::string const & directory, std::string const & file_name);
};

#endif
