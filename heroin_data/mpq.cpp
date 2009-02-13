#include <iostream>
#include "mpq.hpp"

struct libmpq_initialiser_type
{
	libmpq_initialiser_type()
	{
		if(libmpq__init())
			throw nil::exception("Failed to initialise libmpq");
	}

	~libmpq_initialiser_type()
	{
		libmpq__shutdown();
	}
};

namespace
{
	libmpq_initialiser_type libmpq_initialiser;
}

mpq_data::mpq_data():
	archive(0),
	owner(true)
{
}

mpq_data::mpq_data(mpq_data const & other):
	archive(other.archive),
	owner(true)
{
	//what a hack
	other.owner = false;
}

mpq_data::~mpq_data()
{
	if(owner)
		close();
}

bool mpq_data::open(std::string const & mpq_file)
{
	return libmpq__archive_open(&archive, mpq_file.c_str(), 0) == 0;
}

bool mpq_data::read(std::string const & file_name, std::string & output)
{
	if(archive == 0)
		return false;
	uint32_t file_number;
	if(libmpq__file_number(archive, file_name.c_str(), &file_number))
		return false;
	off_t libmpq_file_size;
	if(libmpq__file_unpacked_size(archive, file_number, &libmpq_file_size))
		return false;
	std::size_t file_size = static_cast<std::size_t>(libmpq_file_size);
	char * buffer = new char[file_size];
	off_t bytes_read;
	if(libmpq__file_read(archive, file_number, reinterpret_cast<uint8_t *>(buffer), libmpq_file_size, &bytes_read))
		return false;
	output.assign(buffer, file_size);
	return true;
}

std::vector<std::string> mpq_data::names()
{
	std::vector<std::string> output;
	char buffer[0x100];
	for(uint32_t i = 0, end = archive->files; i < end; i++)
	{
		if(libmpq__file_name(archive, i, buffer, sizeof(buffer)))
		{
			std::cout <<"Failed to read filename of entry " << i << std::endl;
			return output;
		}
		output.push_back(std::string(buffer));
	}
	return output;
}

void mpq_data::close()
{
	if(archive != 0)
		libmpq__archive_close(archive);
}
