#ifndef ITEM_DATA_HPP
#define ITEM_DATA_HPP

#include <string>
#include <map>

#include <heroin/item.hpp>

std::size_t const inventory_width = 10;
std::size_t const inventory_height = 4;

std::size_t const stash_width = 6;
std::size_t const stash_height = 8;

std::size_t const cube_width = 3;
std::size_t const cube_height = 4;

struct item_entry
{
	std::string name;
	std::string type;
	item_classification_type classification;
	ulong width, height;
	bool stackable, useable, throwable;

	item_entry();
	item_entry(std::string const & name, std::string const & type, item_classification_type classification, ulong width, ulong height, bool stackable, bool useable, bool throwable);
	bool is_armor() const;
	bool is_weapon() const;
};

std::string item_quality_to_string(item_quality_type quality);

#endif
