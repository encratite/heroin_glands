#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <set>
#include <string>
#include <heroin/item.hpp>
#include <heroin/types.hpp>

class container
{
public:
	container();
	container(std::string const & name, std::size_t width, std::size_t height);
	void add(item_type const & item);
	void remove(item_type const & item);
	void print_fields() const;
	bool find_free_space(item_type const & item, coordinate & output) const;

private:
	std::string name;
	std::set<item_type> items;
	std::vector<bool_line> fields;
	std::size_t width, height;
	void set_item_fields(item_type const & item, bool value);
	bool rectangle_is_free(std::size_t rectangle_x, std::size_t rectangle_y, std::size_t rectangle_width, std::size_t rectangle_height) const;
};

#endif
