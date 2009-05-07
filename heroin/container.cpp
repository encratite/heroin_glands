#include <iostream>
#include <heroin/container.hpp>
#include <heroin/item_data.hpp>

container::container()
{
}

container::container(std::string const & name, std::size_t width, std::size_t height):
	name(name),
	width(width),
	height(height)
{
	bool_line field_line(width, false);
	fields = std::vector<bool_line>(height, field_line);
}

void container::add(item_type const & item)
{
	items.insert(item);
	set_item_fields(item, true);
}

void container::remove(item_type const & item)
{
	set_item_fields(item, false);
	items.erase(item);
}

void container::print_fields() const
{
	std::cout << name << ":" << std::endl;
	for(std::size_t y = 0; y < height; y++)
	{
		std::cout << "\t[";
		for(std::size_t x = 0; x < width; x++)
			std::cout << (fields[y][x] ? "X" : " ");
		std::cout << "]" << std::endl;
	}
	std::cout << std::endl;
}

bool container::find_free_space(item_type const & item, coordinate & output) const
{
	std::size_t item_width = item.width;
	std::size_t item_height = item.height;
	for(std::size_t y = 0; y < height; y++)
	{
		for(std::size_t x = 0; x < width; x++)
		{
			if(rectangle_is_free(x, y, item.width, item.height))
			{
				output.x = x;
				output.y = y;
				return true;
			}
		}
	}
	return false;
}

void container::set_item_fields(item_type const & item, bool value)
{
	try
	{
		for(std::size_t y = 0; y < item.height; y++)
			for(std::size_t x = 0; x < item.width; x++)
				fields.at(item.y + y).at(item.x + x) = value;
	}
	catch(std::exception &)
	{
		std::cout << "\"" << name << "\" coordinate exception: " << item.name << " (" << item.x << ", " << item.y << ")" << std::endl;
	}
}

bool container::rectangle_is_free(std::size_t rectangle_x, std::size_t rectangle_y, std::size_t rectangle_width, std::size_t rectangle_height) const
{
	if(
		(rectangle_x + rectangle_width > width) ||
		(rectangle_y + rectangle_height > height)
	)
		return false;

	for(std::size_t y = rectangle_y; y < rectangle_y + rectangle_height; y++)
	{
		for(std::size_t x = rectangle_x; x < rectangle_x + rectangle_width; x++)
		{
			if(fields[y][x])
				return false;
		}
	}
	return true;
}
