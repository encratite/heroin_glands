#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include <string>
#include <vector>
#include <heroin/item_data.hpp>
#include <heroin/types.hpp>

class item_data_type
{
public:
	item_data_type();
	item_data_type(std::string const & file);

	bool get(std::string const & code, item_entry & output);

private:
	std::vector<item_entry> items;
};

class plain_text_data_type
{
public:
	plain_text_data_type();
	plain_text_data_type(std::string const & file);

	bool get(std::size_t offset, std::string & output);
	bool get(std::size_t offset, string_vector & output);

private:
	string_vectors lines;
};

class binary_data_type
{
public:
	binary_data_type();
	binary_data_type(std::string const & file);

	bool get(std::size_t offset, std::size_t length, std::string & output);

private:
	std::string data;
};

struct data_manager
{
	data_manager();
	data_manager(std::string plain_text_data_directory);

	item_data_type item_data;
	plain_text_data_type
		magical_prefixes,
		magical_suffixes,
		rare_prefixes,
		rare_suffixes,
		unique_items,
		monster_names,
		monster_fields,
		super_uniques,
		item_properties,
		skills;
};

#endif
