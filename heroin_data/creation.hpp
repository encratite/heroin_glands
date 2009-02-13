#ifndef CREATION_HPP
#define CREATION_HPP

#include <string>
#include "string_table_manager.hpp"

void create_item_data(std::string const & armor_txt, std::string const & misc_txt, std::string const & weapons_txt, string_table_manager & string_manager, std::string const & output_directory);
void create_experience_data(std::string const & experience_txt, std::string const & output_directory);
void create_magical_affix_data(std::string const & magicprefix_txt, std::string const & magicsuffix_txt, string_table_manager & string_manager, std::string const & output_directory);
void create_monster_data(std::string const & monstats_txt, std::string const & monstats2_bin, string_table_manager & string_manager, std::string const & output_directory);
void create_rare_affix_data(std::string const & rareprefix_txt, std::string const & raresuffix_txt, string_table_manager & string_manager, std::string const & output_directory);
void create_set_item_data(std::string const & setitems_txt, string_table_manager & string_manager, std::string const & output_directory);
void create_skills_data(std::string const & skills_txt, std::string const & output_directory);
void create_super_uniques_data(std::string const & superuniques_txt, string_table_manager & string_manager, std::string const & output_directory);
void create_unique_item_data(std::string const & uniqueitems_txt, string_table_manager & string_manager, std::string const & output_directory);
void create_levels_data(std::string const & levels_txt, string_table_manager & string_manager, std::string const & output_directory);
void create_item_property_data(std::string const & properties_txt, std::string const & itemstatcost_txt, std::string const & output_directory);

#endif
