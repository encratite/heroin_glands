#ifndef MPQ_HPP
#define MPQ_HPP

#include <string>
#include <vector>
#include <mpq.h>
#include <nil/exception.hpp>

class mpq_data
{
public:
	mpq_data();
	mpq_data(mpq_data const & other);
	~mpq_data();
	bool open(std::string const & mpq_file);
	bool read(std::string const & file_name, std::string & output);
	void close();
	std::vector<std::string> names();

private:
	mpq_archive_s * archive;
	mutable bool owner;
};

#endif
