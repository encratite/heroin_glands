#include <heroin/client.hpp>

#include <iostream>

#include <nil/array.hpp>
#include <nil/random.hpp>
#include <nil/time.hpp>
#include <nil/string.hpp>

#include <heroin/utility.hpp>
#include <heroin/compression.hpp>

namespace
{
	//stolen from FooSoft

	std::size_t const packet_sizes[] =
	{
		1, 8, 1, 12, 1, 1, 1, 6, 6, 11, 6, 6, 9, 13, 12, 16, 
		16, 8, 26, 14, 18, 11, 0, 0, 15, 2, 2, 3, 5, 3, 4, 6,
		10, 12, 12, 13, 90, 90, 0, 40, 103,97, 15, 0, 8, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 34, 8,
		13, 0, 6, 0, 0, 13, 0, 11, 11, 0, 0, 0, 16, 17, 7, 1,
		15, 14, 42, 10, 3, 0, 0, 14, 7, 26, 40, 0, 5, 6, 38, 5,
		7, 2, 7, 21, 0, 7, 7, 16, 21, 12, 12, 16, 16, 10, 1, 1,
		1, 1, 1, 32, 10, 13, 6, 2, 21, 6, 13, 8, 6, 18, 5, 10,
		4, 20, 29, 0, 0, 0, 0, 0, 0, 2, 6, 6, 11, 7, 10, 33,
		13, 26, 6, 8, 0, 13, 9, 1, 7, 16, 17, 7, 0, 0, 7, 8,
		10, 7, 8, 24, 3, 8, 0, 7, 0, 7, 0, 7, 0, 0, 0, 0, 
		1
	};
}

std::string pad_string(std::string const & name)
{
	return name + std::string(16 - name.length(), '\x00');
}

bool find_next_zero(std::string const & input, std::size_t offset, std::size_t & output)
{
	std::size_t zero_offset = input.find('\x00', offset);
	if(zero_offset == std::string::npos)
		return false;
	output = zero_offset - offset;
	return true;
}

bool get_chat_packet_size(std::string const & input, std::size_t & output)
{
	if(input.size() < 12)
		return false;

	std::size_t const initial_offset = 10;

	std::size_t name_offset;
	if(!find_next_zero(input, initial_offset, name_offset))
		return false;

	std::size_t message_offset;
	if(!find_next_zero(input, initial_offset + name_offset + 1, message_offset))
		return false;

	output = initial_offset + name_offset + 1 + message_offset + 1;

	return true;
}


//from RedVex
bool get_packet_size(std::string const & input, std::size_t & output)
{
	unsigned char const * bytes = reinterpret_cast<const unsigned char*>(input.c_str());
	ulong identifier = get_byte(input, 0);

	std::size_t size = input.size();

	switch(identifier)
	{
	case 0x26:
		if(get_chat_packet_size(input, output))
			return true;
		break;

	case 0x5b:
		if(size >= 3)
		{
			output = static_cast<std::size_t>(read_word(input, 1));
			return true;
		}
		break;

	case 0x94:
		if(size >= 2)
		{
			output = static_cast<std::size_t>(input[1] * 3 + 6);
			return true;
		}
		break;

	case 0xa8:
	case 0xaa:
		if(size >= 7)
		{
			output = static_cast<std::size_t>(bytes[6]);
			return true;
		}
		break;

	case 0xac:
		if(size >= 13)
		{
			output = static_cast<std::size_t>(bytes[12]);
			return true;
		}
		break;

	case 0xae:
		if(size >= 3)
		{
			output = 3 + read_word(input, 1);
			return true;
		}
		break;

	case 0x9c:
		if(size >= 3)
		{
			output = static_cast<std::size_t>(bytes[2]);
			return true;
		}
		break;

	case 0x9d:
		if(size >= 3)
		{
			output = static_cast<std::size_t>(bytes[2]);
			return true;
		}
		break;

	default:
		if(identifier < nil::countof(packet_sizes))
		{
			output = packet_sizes[identifier];
			return output != 0;
		}
		break;
	}

	return false;
}

void d2_client::gs_thread_function(void * unused)
{
	initialise_game_data();

	std::cout << "Connecting to game server " << gs_ip << ":" << gs_port << std::endl;
	if(gs_socket.connect(gs_ip, gs_port))
		std::cout << "Successfully connected to game server" << std::endl;
	else
	{
		std::cout << "Failed to connect to game server" << std::endl;
		return;
	}

	connected_to_gs = true;

	std::string buffer;
	while(true)
	{
		//std::cout << "Waiting for GS data" << std::endl;
		std::string data;
		if(!gs_socket.receive(data))
		{
			std::cout << "Disconnected from game server" << std::endl;
			connected_to_gs = false;
			break;
		}

		//std::cout << "GS data:" << std::endl;
		//print_data(data);

		buffer += data;

		while(true)
		{
			if(buffer.size() >= 2 && buffer.substr(0, 2) == "\xaf\x01")
			{
				std::cout << "Logging on to game server" << std::endl;
				std::string packet = "\x68" + gs_hash + gs_token + byte_to_string(class_byte) + dword_to_string(0xc) + "\x50\xcc\x5d\xed\xb6\x19\xa5\x91" + zero + pad_string(character);
				gs_socket.send(packet);
				buffer.erase(0, 2);
				continue;
			}

			if(buffer.size() < 2 || (static_cast<unsigned char>(buffer[0]) >= 0xF0 && buffer.size() < 3))
			{
				//std::cout << "Buffer lacks data to determine packet size" << std::endl;
				//print_data(buffer);
				break;
			}

			std::size_t header_size;
			std::size_t length = determine_game_packet_size(buffer, header_size);
			length += header_size;
			if(length > buffer.size())
			{
				//std::cout << "Buffer lacks data for next length " << header_size << ", " << length << std::endl;
				//print_data(buffer);
				break;
			}

			std::string packet = buffer.substr(0, length);
			buffer.erase(0, length);

			//std::cout << "Processing packet:" << std::endl;
			//print_data(packet);

			packet = decompress_packet(packet);

			//std::cout << "Decompressed game server packet:" << std::endl;
			//print_data(packet);

			//std::cout << "Splitting packet" << std::endl;

			while(!packet.empty())
			{
				std::size_t packet_size;
				if(!get_packet_size(packet, packet_size))
				{
					std::cout << "Failed to determine packet size:" << std::endl;
					print_data(packet);
				}
				std::string actual_packet = packet.substr(0, packet_size);
				packet.erase(0, packet_size);

				//std::cout << "Actual GS packet:" << std::endl;
				//print_data(actual_packet);

				process_game_packet(actual_packet);
			}
		}
	}
}

void d2_client::ping_thread_function(void * unused)
{
	while(connected_to_gs)
	{
		//std::cout << "Sending ping" << std::endl;
		if(!gs_socket.send("\x6d" + dword_to_string(get_tick_count()) + null + null))
			break;

		//what a hack
		ulong sleep_step = 100;
		for(long i = 0; i < 5000 && connected_to_gs; i += sleep_step)
			nil::sleep(sleep_step);
	}
	std::cout << "Ping thread terminating" << std::endl;
}

void d2_client::process_game_packet(std::string const & packet)
{
	ulong identifier = get_byte(packet, 0);
	switch(identifier)
	{
		case 0x00:
			std::cout << "The game is loading, please wait" << std::endl;
			break;

		case 0x01:
			//game flags, Diablo II client sends ping
			std::cout << "Replying to game flags with ping" << std::endl;
			gs_socket.send("\x6d" + dword_to_string(get_tick_count()) + dword_to_string(get_tick_count() - last_timestamp) + null);
			break;

		case 0x02:
			std::cout << "The game is done loading, joining the game" << std::endl;
			gs_socket.send("\x6b");
			std::cout << "Launching ping thread" << std::endl;
			ping_thread.start(nil::thread::function_type(*this, &d2_client::ping_thread_function));
			break;
		
		case 0x03:
			//load act
			current_act = static_cast<act_type>(get_byte(packet, 1));
			map_id = read_dword(packet, 2);
			area_id = read_word(packet, 6);
			if(!fully_entered_game)
			{
				fully_entered_game = true;
				std::cout << "Entering the game" << std::endl;
				enter_game();
			}
			break;

		case 0x0f:
		{
			//another player is moving
			ulong player_id = read_dword(packet, 2);
			player & current_player = get_player(player_id);
			current_player.location = coordinate(read_word(packet, 7),read_word(packet, 9));
			current_player.directory_known = true;
			player_moved(player_id);
			break;
		}

		case 0x15:
		{
			//player reassignment, happens occasionally
			ulong id = read_dword(packet, 2);
			player & current_player = get_player(id);
			current_player.location = coordinate(read_word(packet, 6), read_word(packet, 8));
			break;
		}

		case 0x18:
			process_life_mana_packet(packet);
			break;

		case 0x1a:
			process_experience(read_byte(packet, 1));
			break;

		case 0x1b:
			process_experience(read_word(packet, 1));
			break;

		case 0x1c:
			process_experience(read_dword(packet, 1));
			break;

		case 0x22:
		{
			//received an item skill level bonus from something currently equipped
			ulong skill = read_word(packet, 7);
			ulong amount = read_byte(packet, 9);
			item_skill_levels[static_cast<skill_type>(skill)] = amount;
			break;
		}

		case 0x26:
		{
			//chat message
			if(get_byte(packet, 1) == 0x01 && get_byte(packet, 3) == 0x02)
			{
				std::string message_player_name(packet.c_str() + 10);
				if(message_player_name != me.name)
				{
					std::string message(packet.c_str() + 10 + message_player_name.length() + 1);
					player & message_player = get_player(message_player_name);
					chat_message(message_player.id, message);
				}
			}
			break;
		}

		case 0x27:
		{
			if(first_npc_info_packet)
				first_npc_info_packet = false;
			else
			{
				//NPC info
				std::cout << "Talking to an NPC" << std::endl;
				talked_to_npc = true;
				ulong id = read_dword(packet, 2);
				//initiate entity chat
				send_packet("\x2f" + one + dword_to_string(id));
			}
			break;
		}

		case 0x5b:
		{
			//a player has joined the game
			ulong new_player_id = read_dword(packet, 3);
			if(new_player_id != me.id)
			{
				std::string const & name = std::string(packet.c_str() + 8);
				character_class_type character_class = static_cast<character_class_type>(get_byte(packet, 7));
				ulong level = read_word(packet, 24);
				player new_player(name, new_player_id, character_class, level);
				players[new_player_id] = new_player;
				player_joined(new_player_id);
			}
			break;
		}

		case 0x5c:
		{
			//a player has left the game
			ulong player_id = read_dword(packet, 1);
			player_left(player_id);
			players.erase(player_id);
		}

		case 0x59:
		{
			//used for initial player assignment
			if(!me.initialised)
			{
				ulong player_id = read_dword(packet, 1);
				character_class_type character_class = static_cast<character_class_type>(read_byte(packet, 5));
				ulong x = read_word(packet, 22);
				ulong y = read_word(packet, 24);
				me = player(character, player_id, character_class, character_level, x, y);
			}
			break;
		}

		case 0x67:
		{
			//NPC is moving to a location
			ulong id = read_dword(packet, 1);
			ulong movement_type = read_byte(packet, 5);
			ulong x = read_word(packet, 6);
			ulong y = read_word(packet, 8);
			bool running;
			if(movement_type == 0x17)
				running = true;
			else if(movement_type == 0x01)
				running = false;
			else
			{
				std::cout << "Invalid NPC movement type detected: " << std::hex << movement_type << std::endl;
				break;
			}
			npc_type & npc = npc_map[id];
			npc.moving = true;
			npc.running = running;
			npc.target_location = coordinate(x, y);
			break;
		}

		case 0x68:
		{
			//NPC is moving to an entity
			ulong id = read_dword(packet, 1);
			ulong movement_type = read_byte(packet, 5);
			ulong x = read_word(packet, 6);
			ulong y = read_word(packet, 8);
			bool running;
			if(movement_type == 0x18)
				running = true;
			else if(movement_type == 0x00)
				running = false;
			else
			{
				std::cout << "Invalid NPC 'move to target' type detected: " << std::hex << movement_type << std::endl;
				break;
			}
			npc_type & npc = npc_map[id];
			npc.moving = true;
			npc.running = running;
			npc.target_location = coordinate(x, y);
			break;
		}

		case 0x6d:
		{
			//NPC has stopped moving
			ulong id = read_dword(packet, 1);
			ulong x = read_word(packet, 5);
			ulong y = read_word(packet, 7);
			ulong life = read_byte(packet, 9);

			npc_type & npc = npc_map[id];
			npc.moving = false;
			npc.location = coordinate(x, y);
			npc.life = life;
			break;
		}

		case 0x81:
		{
			ulong owner_id = read_dword(packet, 4);
			ulong mercenary_id = read_dword(packet, 8);
			player & current_player = get_player(owner_id);
			current_player.has_mercenary = true;
			current_player.mercenary_id = true;
			break;
		}

		case 0x8f:
			//std::cout << "Pong!" << std::endl;
			last_timestamp = get_tick_count();
			break;

		case 0x94:
		{
			//this packet contains all the base skill levels
			ulong skill_count = read_byte(packet, 1);
			std::size_t offset = 6;
			for(ulong i = 0; i < skill_count; i++)
			{
				ulong skill = read_word(packet, offset);
				ulong level = read_byte(packet, offset + 2);
				skill_levels[static_cast<skill_type>(skill)] = level;
				offset += 3;
			}
			std::cout << "Parsed skill levels" << std::endl;
			determine_character_skill_setup();
			break;
		}

		case 0x95:
			process_life_mana_packet(packet);
			break;

		case 0x9c:
		case 0x9d:
			//terrible bit stream shit, item data
			item_action(packet);
			break;

		case 0xac:
			//terrible bit stream shit, NPC assignment
			npc_assignment(packet);
			break;
	}

	//let the derived class handle the packet
	receive_packet(packet);
}
