#ifndef D2CDKEY_HPP
#define D2CDKEY_HPP

#include <string>

#include <nil/types.hpp>

bool hash_d2key(std::string const & cdkey, ulong client_token, ulong server_token, std::string & output, std::string & public_value);

#endif
