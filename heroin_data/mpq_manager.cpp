#include <iostream>
#include <algorithm>
#include <nil/array.hpp>
#include <nil/file.hpp>
#include "mpq_manager.hpp"

mpq_manager::mpq_manager(std::string const & directory)
{
	std::string const mpq_archives[] =
	{
		"Patch_D2.mpq",
		"d2exp.mpq",
		"d2data.mpq"
	};
	for(std::size_t i = 0; i < nil::countof(mpq_archives); i++)
		add_mpq_file(directory, mpq_archives[i]);
}

bool mpq_manager::read(std::string const & file_name, std::string & output)
{
	for(std::size_t i = 0; i < mpq_files.size(); i++)
	{
		bool success = mpq_files[i].read(file_name, output);
		if(success)
		{
			std::cout << "Located " << file_name << " in " << mpq_archive_names[i] << std::endl;
			return true;
		}
	}
	return false;
}

bool mpq_manager::extract_files(std::string const & file_name, std::string const & output_directory)
{
	bool found_one = false;
	for(std::size_t i = 0; i < mpq_files.size(); i++)
	{
		std::string output;
		bool success = mpq_files[i].read(file_name, output);
		if(success)
		{
			std::string file;
			std::size_t offset = file_name.rfind('\\');
			if(offset == std::string::npos)
				file = file_name;
			else
				file = file_name.substr(offset + 1);
			std::string const & mpq_name = mpq_archive_names[i];
			std::string target = output_directory + mpq_name + "_" + file;
			std::cout << "Writing data to " << target << std::endl;
			nil::write_file(target, output);
			found_one = true;
		}
	}
	return found_one;
}

bool mpq_manager::read(std::string const & mpq_name, std::string const & file_name, std::string & output)
{
	for(std::size_t i = 0; i < mpq_files.size(); i++)
	{
		if(mpq_files[i].read(file_name, output))
		{
			std::cout << "File exists in " << mpq_archive_names[i] << " (" << output.size() << " bytes)" << std::endl;
			if(i == 0)
			{
				std::string name = file_name.substr(file_name.rfind('\\') + 1);
				nil::write_file("D:\\patch_d2\\" + name, output);
			}
		}
	}

	for(std::size_t i = 0; i < mpq_files.size(); i++)
	{
		if(mpq_archive_names[i] == mpq_name)
			return mpq_files[i].read(file_name, output);
	}
	return false;
}

void mpq_manager::add_mpq_file(std::string const & directory, std::string const & file_name)
{
	mpq_data new_data;
	std::cout << "Loading MPQ file " << file_name << std::endl;
	if(new_data.open(directory + file_name))
	{
		std::cout << "Successfully loaded file" << std::endl;
		mpq_files.push_back(new_data);
		mpq_archive_names.push_back(file_name);
	}
	else
		std::cout << "Failed to load file" << std::endl;
}
