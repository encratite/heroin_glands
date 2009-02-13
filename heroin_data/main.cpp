#include <iostream>
#include <string>
#include <cstdlib>
#include <nil/file.hpp>
#include <nil/array.hpp>
#include <nil/string.hpp>
#include "mpq_manager.hpp"
#include "string_table_manager.hpp"
#include "creation.hpp"
#include "utility.hpp"

namespace
{
	std::string const
		mpq_all = "",
		patch_d2 = "Patch_D2.mpq",
		d2exp = "d2exp.mpq",
		d2data = "d2data.mpq";

	std::string const
		excel_prefix = "data\\global\\excel\\",
		table_prefix = "data\\local\\LNG\\ENG\\";
}

std::string get_error_string(ulong error)
{
	switch(error)
	{
		case LIBMPQ_ERROR_NOT_INITIALIZED:
			return "Library has not been initialised";
		case LIBMPQ_ERROR_OPEN:
			return "Failed to open archive";
		case LIBMPQ_ERROR_CLOSE:
			return "Failed to close file";
		case LIBMPQ_ERROR_MALLOC:
			return "Not enough memory to allocate required structures";
		case LIBMPQ_ERROR_SEEK:
			return "Failed to seek data in file";
		case LIBMPQ_ERROR_FORMAT:
			return "Invalid MPQ file";
		case LIBMPQ_ERROR_READ:
			return "Failed to read from MPQ file";
		case LIBMPQ_ERROR_SIZE:
			return "Buffer size too small";
		case LIBMPQ_ERROR_EXIST:
			return "File or block does not exist in archive";
		case LIBMPQ_ERROR_DECRYPT:
			return "Unknown encryption seed";
		case LIBMPQ_ERROR_UNPACK:
			return "Failed to unpack data";
		default:
			return "Unknown error occured (" + nil::string::number_to_string<ulong>(error) + ")";
	}
}

void process_error(ulong error)
{
	std::cout << "Error: " << get_error_string(error) << std::endl;
	exit(1);
}

void extract_mpq_files(std::string const & mpq_path, std::string const & output_directory)
{
	mpq_data mpq;
	if(!mpq.open(mpq_path))
	{
		std::cout << "Failed to open MPQ file" << std::endl;
		return;
	}
	std::vector<std::string> files = mpq.names();
	for(std::size_t i = 0; i < files.size(); i++)
	{
		std::cout << (i + 1) << ". " << files[i] << std::endl;
	}
}

struct mpq_file_entry
{
	std::string const & mpq_name;
	std::string path;
	std::string & output;

	mpq_file_entry(std::string const & mpq_name, std::string const & path, std::string & output):
		mpq_name(mpq_name),
		path(path),
		output(output)
	{
	}
};

void print_usage(char ** argv)
{
	std::cout << argv[0] << " <Diablo II directory> <output data directory>" << std::endl;
	std::cout << argv[0] << " <Diablo II directory> search <string>" << std::endl;
	std::cout << argv[0] << " <Diablo II directory> extract <MPQ internal path> <output data directory>" << std::endl;
}

void perform_search(std::string const & target, std::string const & data, std::string const & file_name)
{
	std::size_t offset = data.find(target);
	if(offset == std::string::npos)
		std::cout << "Not found in " << file_name << std::endl;
	else
		std::cout << "Found entry at " << offset << " in " << file_name << std::endl;
}

bool load_mpq_data(mpq_manager & archive_manager, mpq_file_entry * entries, std::size_t count)
{
	for(std::size_t i = 0; i < count; i++)
	{
		mpq_file_entry & entry = entries[i];
		std::cout << "Loading " << entry.path;

		bool result;
		if(entry.mpq_name.empty())
		{
			std::cout << std::endl;
			result = archive_manager.read(entry.path, entry.output);
		}
		else
		{
			std::cout << " from " << entry.mpq_name << std::endl;
			result = archive_manager.read(entry.mpq_name, entry.path, entry.output);
		}

		if(result)
			std::cout << "Loaded " << entry.path << std::endl;
		else
		{
			std::cout << "Failed to load " << entry.path << std::endl;
			return false;
		}
	}
	return true;
}

int main(int argc, char ** argv)
{
	std::string
		armor_txt,
		experience_txt,
		gems_txt,
		itemstatcost_txt,
		misc_txt,
		levels_txt,
		monstats_txt,
		monstats2_bin,
		magicprefix_txt,
		magicsuffix_txt,
		rareprefix_txt,
		raresuffix_txt,
		runes_txt,
		properties_txt,
		setitems_txt,
		skills_txt,
		superuniques_txt,
		uniqueitems_txt,
		weapons_txt,

		patchstring_tbl,
		expansionstring_tbl,
		string_tbl;

	if(argc < 3)
	{
		print_usage(argv);
		return 1;
	}

	std::string directory(argv[1]);

	std::string second_argument(argv[2]);

	//extract_mpq_files(directory + patch_d2, "D:\\patch_d2\\");

	mpq_manager archive_manager(directory);

	mpq_file_entry data_entries[] =
	{
		mpq_file_entry(mpq_all, excel_prefix + "armor.txt", armor_txt),
		mpq_file_entry(mpq_all, excel_prefix + "experience.txt", experience_txt),
		//mpq_file_entry(mpq_all, excel_prefix + "gems.txt", gems_txt),
		mpq_file_entry(mpq_all, excel_prefix + "ItemStatCost.txt", itemstatcost_txt),
		mpq_file_entry(mpq_all, excel_prefix + "misc.txt", misc_txt),
		mpq_file_entry(mpq_all, excel_prefix + "levels.txt", levels_txt),

		mpq_file_entry(mpq_all, excel_prefix + "monstats.txt", monstats_txt),
		mpq_file_entry(mpq_all, excel_prefix + "monstats2.bin", monstats2_bin),

		mpq_file_entry(mpq_all, excel_prefix + "magicprefix.txt", magicprefix_txt),
		mpq_file_entry(mpq_all, excel_prefix + "magicsuffix.txt", magicsuffix_txt),

		mpq_file_entry(mpq_all, excel_prefix + "properties.txt", properties_txt),

		mpq_file_entry(mpq_all, excel_prefix + "rareprefix.txt", rareprefix_txt),
		mpq_file_entry(mpq_all, excel_prefix + "raresuffix.txt", raresuffix_txt),

		//mpq_file_entry(mpq_all, excel_prefix + "runes.txt", runes_txt),
		mpq_file_entry(mpq_all, excel_prefix + "setitems.txt", setitems_txt),
		mpq_file_entry(mpq_all, excel_prefix + "skills.txt", skills_txt),
		mpq_file_entry(mpq_all, excel_prefix + "superuniques.txt", superuniques_txt),
		mpq_file_entry(mpq_all, excel_prefix + "uniqueitems.txt", uniqueitems_txt),
		mpq_file_entry(mpq_all, excel_prefix + "weapons.txt", weapons_txt)
	};

	mpq_file_entry string_entries[] =
	{
		mpq_file_entry(mpq_all, table_prefix + "patchstring.tbl", patchstring_tbl),
		mpq_file_entry(mpq_all, table_prefix + "expansionstring.tbl", expansionstring_tbl),
		mpq_file_entry(mpq_all, table_prefix + "string.tbl", string_tbl),
	};

	if(second_argument == "search")
	{
		if(argc != 4)
		{
			std::cout << "Invalid argument count for string table search" << std::endl;
			print_usage(argv);
			return 1;
		}
		std::string target(argv[3]);

		if(!load_mpq_data(archive_manager, string_entries, nil::countof(string_entries)))
			return 1;

		string_table_manager string_manager(patchstring_tbl, expansionstring_tbl, string_tbl);
		perform_search(target, patchstring_tbl, "patchstring.tbl");
		perform_search(target, expansionstring_tbl, "expansionstring.tbl");
		perform_search(target, string_tbl, "string.tbl");

		string_manager.debug = true;
		string_manager.transform(target);
	}
	else if(second_argument == "extract")
	{
		if(argc != 5)
		{
			std::cout << "Invalid argument count for MPQ file extraction" << std::endl;
			print_usage(argv);
			return 1;
		}
		std::string file_name(argv[3]);
		std::string output_directory(argv[4]);
		if(!archive_manager.extract_files(file_name, output_directory))
			std::cout << "Unable to find file" << std::endl;
	}
	else
	{
		initialise_mod_map();
		initialise_stat_cost_map();

		if
		(
			!load_mpq_data(archive_manager, data_entries, nil::countof(data_entries)) ||
			!load_mpq_data(archive_manager, string_entries, nil::countof(string_entries))
		)
			return 1;

		string_table_manager string_manager(patchstring_tbl, expansionstring_tbl, string_tbl);

		std::string output_directory(argv[2]);
		create_item_data(armor_txt, misc_txt, weapons_txt, string_manager, output_directory);
		create_experience_data(experience_txt, output_directory);
		create_magical_affix_data(magicprefix_txt, magicsuffix_txt, string_manager, output_directory);
		create_monster_data(monstats_txt, monstats2_bin, string_manager, output_directory);
		create_rare_affix_data(rareprefix_txt, raresuffix_txt, string_manager, output_directory);
		create_set_item_data(setitems_txt, string_manager, output_directory);
		create_skills_data(skills_txt, output_directory);
		create_super_uniques_data(superuniques_txt, string_manager, output_directory);
		create_unique_item_data(uniqueitems_txt, string_manager, output_directory);
		create_levels_data(levels_txt, string_manager, output_directory);
		create_item_property_data(properties_txt, itemstatcost_txt, output_directory);
	}
	return 0;
}
