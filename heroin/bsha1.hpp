#ifndef BSHA1_HPP
#define BSHA1_HPP

#include <string>

#include <nil/types.hpp>

std::string bsha1(std::string const & input);
std::string double_hash(ulong client_token, ulong server_token, std::string const & password);

#endif
