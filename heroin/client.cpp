#include <heroin/client.hpp>

#include <iostream>
#include <cmath>

#include <nil/string.hpp>
#include <nil/time.hpp>

#include <heroin/utility.hpp>
#include <heroin/item_data.hpp>

extern std::string const
	null("\x00\x00\x00\x00", 4),
	one("\x01\x00\x00\x00", 4),
	zero("\x00", 1),
	platform("68XI"),
	classic_id("VD2D"),
	lod_id("PX2D");

d2_client::d2_client(data_manager & client_data_manager):
	status(d2_client_status_idle),
	client_data_manager(client_data_manager),
	chicken_life(0),
	debugging(false)
{
}

d2_client::~d2_client()
{
	terminate();
}

void d2_client::set_binary_information(std::string const & new_d2_binaries_directory, std::string const & new_game_exe_information)
{
	d2_binaries_directory = new_d2_binaries_directory;
	game_exe_information = new_game_exe_information;

	fix_directory(d2_binaries_directory);
}

void d2_client::set_realm_information(std::string const & new_battle_net_server, std::string const & new_realm)
{
	battle_net_server = new_battle_net_server;
	realm = new_realm;
}

void d2_client::set_key_data(std::string const & new_classic_key, std::string const & new_lod_key, std::string const & new_key_owner)
{
	classic_key = new_classic_key;
	lod_key = new_lod_key;
	key_owner = new_key_owner;
}

void d2_client::set_login_data(std::string const & new_account, std::string const & new_password, std::string const & new_character)
{
	account = new_account;
	password = nil::string::to_lower(new_password);
	character = new_character;
}

void d2_client::set_game_data(bool new_create_game, std::string const & new_game_name, std::string const & new_game_password)
{
	create_game = new_create_game;
	game_name = new_game_name;
	game_password = new_game_password;
}

void d2_client::set_inventory_settings(std::vector<std::string> const & lines)
{
	std::vector<bool_line> new_inventory_settings;
	if(lines.size() != inventory_height)
		throw nil::exception("Invalid inventory height in inventory settings");
	for(std::size_t i = 0; i < inventory_height; i++)
	{
		std::string const & line = lines[i];
		if(line.size() != inventory_width)
			throw nil::exception("Invalid inventory width on at least one line of the inventory settings");
		bool_line new_line;
		for(std::size_t j = 0; j < inventory_width; j++)
		{
			char letter = line[j];
			switch(letter)
			{
				case '0':
					new_line.push_back(false);
					break;

				case '1':
					new_line.push_back(true);
					break;

				default:
					throw nil::exception("Encountered invalid byte in inventory settings string");
			}
		}
		new_inventory_settings.push_back(new_line);
	}
	inventory_settings = new_inventory_settings;
}

void d2_client::set_difficulty(std::string const & new_difficulty)
{
	std::string difficulty_string = nil::string::to_lower(new_difficulty);
	if(difficulty_string == "hell")
		difficulty = game_difficulty::hell;
	else if(difficulty_string == "nightmare")
		difficulty = game_difficulty::nightmare;
	else if(difficulty_string == "normal")
		difficulty = game_difficulty::normal;
	else
	{
		std::cout << "Unable to parse difficulty \"" << new_difficulty << "\", defaulting to normal" << std::endl;
		difficulty = game_difficulty::normal;
	}
}

void d2_client::set_chicken_life(ulong new_chicken_life)
{
	chicken_life = new_chicken_life;
}

void d2_client::launch()
{
	terminate();
	status = d2_client_status_active;
	bncs_thread.start(nil::thread::function_type(*this, &d2_client::bncs_thread_function));
}

void d2_client::terminate()
{
	if(status == d2_client_status_active)
	{
		std::cout << "Terminating client" << std::endl;

		status = d2_client_status_terminating;

		bncs_socket.disconnect();
		mcp_socket.disconnect();
		gs_socket.disconnect();

		join_thread(bncs_thread, "BNCS");
		join_thread(mcp_thread, "MCP");
		join_thread(gs_thread, "GS");
		join_thread(ping_thread, "ping");

		if(debugging)
			std::cout << "Client has terminated" << std::endl;

		status = d2_client_status_idle;
	}
}

void d2_client::enter_game()
{
}

void d2_client::receive_packet(std::string const & packet)
{
}

void d2_client::experience_gained(ulong amount)
{
}

void d2_client::player_moved(ulong player_id)
{
}

void d2_client::chat_message(ulong player_id, std::string const & message)
{
}

void d2_client::player_left(ulong player_id)
{
}

void d2_client::player_joined(ulong player_id)
{
}

void d2_client::new_npc(npc_type & npc)
{
}

void d2_client::send_packet(std::string const & packet)
{
	gs_socket.send(packet);
}

void d2_client::initialise_game_data()
{
	fully_entered_game = false;

	last_teleport = 0;
	experience = 0;

	me = player();

	skill_levels.clear();
	item_skill_levels.clear();
	players.clear();

	inventory = container("Inventory", inventory_width, inventory_height);
	stash = container("Stash", stash_width, stash_height);
	cube =  container("Cube", cube_width, cube_height);

	malah_id = 0;
	current_life = 0;
	first_npc_info_packet = true;
}

void d2_client::process_experience(ulong gained_experience)
{
	bool first_time = (experience == 0);
	experience += gained_experience;
	if(!first_time)
		experience_gained(gained_experience);
}

player & d2_client::get_player(ulong id)
{
	if(id == me.id)
		return me;
	else
		return players[id];
}

player & d2_client::get_player(std::string const & name)
{
	for(player_map::iterator i = players.begin(); i != players.end(); i++)
	{
		player & current_player = i->second;
		if(current_player.name == name)
			return current_player;
	}
	return me;
}

ulong d2_client::get_skill_level(skill_type skill)
{
	return skill_levels[skill] + item_skill_levels[skill];
}

void d2_client::join_game(std::string const & join_game_name, std::string const & join_game_password)
{
	mcp_socket.send(construct_mcp_packet(0x04, word_to_string(game_request_id) + game_name + zero + game_password + zero));
	bncs_socket.send(construct_bncs_packet(0x1c, dword_to_string(1) + null + null + null + null + game_name + zero + game_password + zero + zero));
}

void d2_client::request_reassignment()
{
	send_packet(std::string("\x4b\x00\x00\x00\x00", 5) + dword_to_string(me.id));
}

bool d2_client::find_stash_space(item_type const & item, coordinate & output)
{
	return stash.find_free_space(item, output);
}

bool d2_client::talk_to_trader(ulong id)
{
	std::cout << "Attempting to talk to NPC" << std::endl;

	talked_to_npc = false;
	npc_type & npc = npc_map[id];
	double distance = me.location.distance(npc.location);
	//make NPC move to us
	send_packet("\x59" + one + dword_to_string(id) + dword_to_string(me.location.x) + dword_to_string(me.location.y));
	long const sleep_step = 200;
	std::string move_to_entity = "\x04" + one + dword_to_string(id);
	//120 is more than necessary, should also be fixed for FRW
	for(long time_difference = static_cast<long>(distance * 120); time_difference > 0; time_difference -= sleep_step)
	{
		//the Diablo II client sends the 'move to entity' packet every 200-300 ms or so, based on the distance to the NPC
		send_packet(move_to_entity);
		nil::sleep(std::min(sleep_step, time_difference));
	}
	//let's hope we're at the NPC now, talk to them
	send_packet("\x13" + one + dword_to_string(id));

	//polling - what a fucking hack :( should use proper signaling...at least it's very portable
	long const timeout_step = 100;
	for(long npc_timeout = 2000; npc_timeout > 0 && !talked_to_npc && status == d2_client_status_active; npc_timeout -= timeout_step)
		nil::sleep(timeout_step);

	if(!talked_to_npc)
	{
		std::cout << "Failed to talk to NPC!" << std::endl;
		return false;
	}

	return true;
}

bool d2_client::talk_to_healer(ulong id)
{
	//unlike talk_to_trader this method immediately terminates the conversation with the NPC, used for healing only
	if(!talk_to_trader(id))
		return false;

	//terminate conversation
	send_packet("\x30" + one + dword_to_string(id));

	return true;
}

void d2_client::leave_game()
{
	std::cout << "Leaving the game" << std::endl;
	send_packet("\x69");
	//gs_socket.disconnect();

	join_thread(gs_thread, "GS");
	join_thread(ping_thread, "ping");
}

void d2_client::move_to(coordinate const & target)
{
	ulong time = nil::time();
	if(time - last_teleport > 5)
	{
		send_packet("\x5f" + word_to_string(target.x) + word_to_string(target.y));
		last_teleport = time;
		nil::sleep(120);
	}
	else
	{
		//Manhattan distance
		//ulong distance = std::abs(target.x - me.x) + std::abs(target.y - me.y);
		//Euclidean distance
		double distance = me.location.distance(target);
		std::cout << me.location.string() << std::endl;
		std::cout << "Euclidean distance to move: " << distance << std::endl;
		send_packet("\x03" + word_to_string(target.x) + word_to_string(target.y));
		//requires FRW fix for better performance, this is for 0%
		nil::sleep(static_cast<ulong>(distance * 100));
	}
	me.location = target;
}

void d2_client::determine_character_skill_setup()
{
	std::cout << "Attempting to detect character skill setup automatically" << std::endl;
	//std::cout << me.character_class << std::endl;
	if(me.character_class == character_class::sorceress)
	{
		if(skill_levels[skill::blizzard] >= 15 && skill_levels[skill::glacial_spike] >= 8 && skill_levels[skill::ice_blast] >= 8)
		{
			std::cout << "Using Blizzard/Glacial Spike/Ice Blast Sorceress setup" << std::endl;
			character_skill_setup = character_skill_setup::blizzard_glacial_spike_ice_blast_sorceress;
		}
		else if(skill_levels[skill::meteor] >= 15 && skill_levels[skill::fire_ball] >= 15)
		{
			std::cout << "Using Meteor/Fireball Sorceress setup" << std::endl;
			character_skill_setup = character_skill_setup::meteor_fireball_sorceress;
		}
		else if(skill_levels[skill::lightning] >= 15 && skill_levels[skill::chain_lightning])
		{
			std::cout << "Using Lightning/Chain Lightning Sorceress setup" << std::endl;
			character_skill_setup = character_skill_setup::lightning_chain_lightning_sorceress;
		}
		else
		{
			std::cout << "Unknown Sorceress skill setup" << std::endl;
			character_skill_setup = character_skill_setup::unknown_setup;
		}
	}
	else
	{
		std::cout << "No configuration available for this character class" << std::endl;
		character_skill_setup = character_skill_setup::unknown_setup;
	}
}

void d2_client::process_life_mana_packet(std::string const & data)
{
	unsigned life = read_word(data, 2) & 0x7fff;
	if(life < current_life && life > 0)
	{
		unsigned damage = current_life - life;
		std::cout << damage << " damage is dealt to " << character << " (" << life << " left)" << std::endl;
		if(chicken_life > 0 && life <= chicken_life)
		{
			std::cout << "Chickening with " << life << " left!" << std::endl;
			leave_game();
		}
	}
	current_life = life;
	if(life == 0)
	{
		std::cout << character << " has " << current_life << " life " << std::endl;
		if(chicken_life == 0)
			std::cout << "Chicken is deactivated" << std::endl;
		else
			std::cout << "Chicken life is set to " << chicken_life << std::endl;
	}
}

void d2_client::join_thread(nil::thread & thread, std::string const & description)
{
	if(debugging)
		std::cout << "Joining thread: " << description << std::endl;
	if(!thread.join() && debugging)
		std::cout << "Thread had already terminated: " << description << std::endl;
}

std::string d2_client::get_skill_name(ulong skill)
{
	std::string output;
	if(!client_data_manager.skills.get(static_cast<std::size_t>(skill), output))
		return "Unknown Skill";
	return output;
}
