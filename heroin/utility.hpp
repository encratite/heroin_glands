#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>

#include <nil/types.hpp>

std::string get_data_string(std::string const & data);
void print_data(std::string const & data);
void print_dword(ulong dword);
std::string get_dword_string(ulong dword);

ulong read_dword(std::string const & input, std::size_t offset);
ulong read_word(std::string const & input, std::size_t offset);
ulong read_byte(std::string const & input, std::size_t offset);
ulong read_nbo_dword(std::string const & input, std::size_t offset);
ulong read_nbo_word(std::string const & input, std::size_t offset);
ulong char_to_byte(char input);
std::string dword_to_string(ulong input);
std::string word_to_string(ulong input);
std::string byte_to_string(ulong input);
std::string char_to_string(char input);
ulong get_byte(std::string const & input, std::size_t offset);
ulong get_tick_count();

#endif