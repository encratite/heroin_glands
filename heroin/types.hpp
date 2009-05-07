#ifndef TYPES_HPP
#define TYPES_HPP

#include <map>
#include <deque>

#include <nil/types.hpp>
#include <heroin/skills.hpp>
#include <heroin/game.hpp>

typedef std::map<ulong, player> player_map;
typedef std::map<skill_type, ulong> skill_map;
typedef std::deque<bool> bool_line;
typedef std::map<ulong, npc_type> npc_map_type;
typedef std::vector<std::string> string_vector;
typedef std::vector<string_vector> string_vectors;

#endif
