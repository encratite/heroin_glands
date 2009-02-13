#include "client.hpp"

#include <iostream>

#include <nil/array.hpp>
#include <nil/random.hpp>
#include <nil/time.hpp>
#include <nil/string.hpp>

#include "utility.hpp"
#include "check_revision.hpp"
#include "bsha1.hpp"
#include "d2cdkey.hpp"
#include "compression.hpp"

namespace
{
	unsigned char const auth_info_packet[] =
	{
		0xff, 0x50, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00, 
		0x36, 0x38, 0x58, 0x49, 0x50, 0x58, 0x32, 0x44, 
		0x0c, 0x00, 0x00, 0x00, 0x53, 0x55, 0x6e, 0x65, 
		0x55, 0xb4, 0x47, 0x40, 0x88, 0xff, 0xff, 0xff, 
		0x09, 0x04, 0x00, 0x00, 0x09, 0x04, 0x00, 0x00, 
		0x55, 0x53, 0x41, 0x00, 0x55, 0x6e, 0x69, 0x74, 
		0x65, 0x64, 0x20, 0x53, 0x74, 0x61, 0x74, 0x65, 
		0x73, 0x00
	};

	//stolen from FooSoft

	std::size_t const packet_sizes[] =
	{
		1,	8,	1,	12,	1,	1,	1,	6,	6,	11,	6,	6,	9,	13,	12,	16, 
		16,	8,	26,	14,	18,	11,	0,	0,	15,	2,	2,	3,	5,	3,	4,	6,
		10,	12,	12,	13,	90,	90,	0,	40,	103,97,	15,	0,	8,	0,	0,	0,
		0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,	8,
		13,	0,	6,	0,	0,	13,	0,	11,	11,	0,	0,	0,	16,	17,	7,	1,
		15,	14,	42,	10,	3,	0,	0,	14,	7,	26,	40,	0,	5,	6,	38,	5,
		7,	2,	7,	21,	0,	7,	7,	16,	21,	12,	12,	16,	16,	10,	1,	1,
		1,	1,	1,	32,	10,	13,	6,	2,	21,	6,	13,	8,	6,	18,	5,	10,
		4,	20,	29,	0,	0,	0,	0,	0,	0,	2,	6,	6,	11,	7,	10,	33,
		13,	26,	6,	8,	0,	13,	9,	1,	7,	16,	17,	7,	0,	0,	7,	8,
		10,	7,	8,	24,	3,	8,	0,	7,	0,	7,	0,	7,	0,	0,	0,	0, 
		1
	};

	ulong const bncs_port = 6112;
	ulong const gs_port = 4000;

	std::string const null("\x00\x00\x00\x00", 4);
	std::string const zero("\x00", 1);
	std::string const platform("68XI");
	std::string const classic_id("VD2D");
	std::string const lod_id("PX2D");
}

std::string pad_string(std::string const & name)
{
	return name + std::string(16 - name.length(), '\x00');
}

std::string get_character_class_string(ulong class_identifier)
{
	switch(class_identifier)
	{
		case 1: return "Amazon";
		case 2: return "Sorceress";
		case 3: return "Necromancer";
		case 4: return "Paladin";
		case 5: return "Barbarian";
		case 6: return "Druid";
		case 7: return "Assassin";
		default: return "Unknown";
	}
}

char get_upper_case_letter()
{
	return static_cast<char>('A' + nil::random::uint(0, 25));
}

char get_lower_case_letter()
{
	return static_cast<char>('a' + nil::random::uint(0, 25));
}

char get_digit()
{
	return static_cast<char>('0' + nil::random::uint(0, 9));
}

char get_letter(bool first)
{
	if(nil::random::uint(1, 36) <= 10)
		return get_digit();
	else
		return first ? get_upper_case_letter() : get_lower_case_letter();
}

std::string generate_string()
{
	std::string output;
	for(long i = 0; i < 15; i++)
		output += get_letter(i == 0);
	return output;
}

std::string read_string(std::string const & packet, std::size_t & offset)
{
	std::size_t zero = packet.find('\x00', offset);
	std::string output;
	if(zero == std::string::npos)
	{
		zero = packet.length();
		output = packet.substr(offset, zero - offset);
		offset = zero;
	}
	else
	{
		output = packet.substr(offset, zero - offset);
		offset = zero + 1;
	}
	return output;
}

std::string read_one_string(std::string const & packet, std::size_t offset)
{
	return read_string(packet, offset);
}

std::string construct_bncs_packet(ulong command, std::string const & arguments)
{
	std::string packet = "\xff" + byte_to_string(command) + word_to_string(4 + arguments.length()) + arguments;
	return packet;
}

std::string construct_mcp_packet(ulong command, std::string const & arguments)
{
	std::string packet = word_to_string(3 + arguments.length()) + byte_to_string(command) + arguments;
	return packet;
}

bool get_bncs_packet(nil::net::tcp_socket & bncs_socket, std::string & buffer, std::string & data)
{
	while(true)
	{
		std::string packet;
		if(!bncs_socket.receive(packet))
			return false;

		buffer += packet;
		if(buffer.size() < 4)
			continue;

		ulong packet_size = read_word(buffer, 2);
		if(packet_size > buffer.size())
			continue;

		data = buffer.substr(0, packet_size);
		buffer.erase(0, packet_size);
		return true;
	}
}

bool get_mcp_packet(nil::net::tcp_socket & mcp_socket, std::string & buffer, std::string & data)
{
	while(true)
	{
		std::string packet;
		if(!mcp_socket.receive(packet))
			return false;

		buffer += packet;
		if(buffer.size() < 3)
			continue;

		ulong packet_size = read_word(buffer, 0);
		if(packet_size > buffer.size())
			continue;

		data = buffer.substr(0, packet_size);
		buffer.erase(0, packet_size);
		return true;
	}
}

d2_client::d2_client():
	status(d2_client_status_idle)
{
}

void d2_client::set_binary_information(std::string const & new_d2_location, std::string const & new_game_exe_information)
{
	d2_location = new_d2_location;
	game_exe_information = new_game_exe_information;

	if(!d2_location.empty())
	{
		char last_byte = *(d2_location.end() - 1);
		if(last_byte != '/' && last_byte != '\\')
			d2_location += "\\";
	}
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
	password = new_password;
	character = new_character;
}

void d2_client::set_difficulty(std::string const & new_difficulty)
{
	std::string difficulty_string = nil::string::to_lower(new_difficulty);
	if(difficulty_string == "hell")
		difficulty = game_difficulty_hell;
	else if(difficulty_string == "nightmare")
		difficulty = game_difficulty_nightmare;
	else if(difficulty_string == "normal")
		difficulty = game_difficulty_normal;
	else
	{
		std::cout << "Unable to parse difficulty \"" << new_difficulty << "\", defaulting to normal" << std::endl;
		difficulty = game_difficulty_normal;
	}
}

ulong convert_game_difficulty(game_difficulty_type difficulty)
{
	switch(difficulty)
	{
		default:
		case game_difficulty_normal:
			return 0x0000;

		case game_difficulty_nightmare:
			return 0x1000;

		case game_difficulty_hell:
			return 0x2000;
	}
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
		status = d2_client_status_terminating;

		bncs_socket.disconnect();
		mcp_socket.disconnect();
		gs_socket.disconnect();

		bncs_thread.join();
		mcp_thread.join();
		gs_thread.join();

		status = d2_client_status_idle;
	}
}

void d2_client::receive_packet(std::string const & packet)
{
}

void d2_client::send_packet(std::string const & packet)
{
	gs_socket.send(packet);
}

void d2_client::bncs_thread_function(void * unused)
{
	std::cout << "Connecting to " << battle_net_server << ":" << bncs_port << "..." << std::endl;
	if(bncs_socket.connect(battle_net_server, bncs_port))
		std::cout << "Connected to BNCS" << std::endl;
	else
	{
		std::cout << "Failed to connect to BNCS" << std::endl;
		return;
	}

	bncs_socket.send("\x01");
	bncs_socket.send(auth_info_packet, nil::countof(auth_info_packet));

	ulong server_token;

	std::string bncs_buffer;

	while(true)
	{
		std::string data;
		if(!get_bncs_packet(bncs_socket, bncs_buffer, data))
			break;

		//std::cout << "BNCS data: " << data << std::endl;
		//print_data(data);

		ulong type = get_byte(data, 1);
		if(type == 0x00 || type == 0x25)
		{
			//ping
			std::cout << "Replying to ping" << std::endl;
			bncs_socket.send(data);
		}
		else if(type == 0x50)
		{
			server_token = read_dword(data, 8);
			std::string mpq_file_time = data.substr(16, 8);

			std::size_t offset = 24;
			std::string mpq_file_name = read_string(data, offset);
			std::string formula_string = read_string(data, offset);

			std::cout << "Received EXE/CD key hash request" << std::endl;
			std::cout << "Server token: " << get_dword_string(server_token) << std::endl;
			std::cout << "MPQ file time: " << get_data_string(mpq_file_time) << std::endl;
			std::cout << "MPQ file name: " << mpq_file_name << std::endl;
			std::cout << "Formula: " << formula_string << std::endl;

			//imitate the Diablo II client and download the MPQ file in question via BNFTP

			nil::net::tcp_socket bnftp_socket;

			if(bnftp_socket.connect(battle_net_server, bncs_port))
				std::cout << "Connected to BNCS for BNFTP transfer" << std::endl;
			else
			{
				std::cout << "Failed to connect to BNCS for BNFTP transfer" << std::endl;
				return;
			}

			bnftp_socket.send("\x02");
			bnftp_socket.send(std::string("\x2f\x00\x00\x01", 4) + platform + classic_id + null + null + null + mpq_file_time + mpq_file_name + zero);

			//read file size
			std::string bnftp_buffer;
			ulong bytes_received = 0;

			do
			{
				bnftp_socket.receive(data);
				bnftp_buffer += data;
				bytes_received += data.length();
			}
			while(bytes_received < 8);

			ulong header_size = read_word(bnftp_buffer, 0);
			ulong file_size = read_dword(bnftp_buffer, 4);
			ulong total_size = header_size + file_size;

			std::cout << "Starting BNFTP download" << std::endl;

			//start BNFTP download
			do
			{
				if(!bnftp_socket.receive(data))
					break;
				bytes_received += data.length();
			}
			while(bytes_received < total_size);

			std::cout << "Finished BNFTP download" << std::endl;

			bnftp_socket.disconnect();

			ulong exe_checksum;
			switch(check_revision(formula_string, mpq_file_name, d2_location, exe_checksum))
			{
				case check_revision_result_success:
					std::cout << "Successfully generated EXE checksum" << std::endl;
					break;

				case check_revision_result_formula_error:
					std::cout << "Unable to parse formula string" << std::endl;
					return;

				case check_revision_result_mpq_error:
					std::cout << "Unable to parse MPQ file name" << std::endl;
					return;

				case check_revision_result_file_error:
					std::cout << "Unable to read D2 binaries - you probably specified the wrong path in the ini file" << std::endl;
					return;

				default:
					std::cout << "Unknown check revision result" << std::endl;
					return;
			}

			ulong client_token = get_tick_count();

			std::string classic_hash, lod_hash, classic_public, lod_public;

			if(hash_d2key(classic_key, client_token, server_token, classic_hash, classic_public))
				std::cout << "Successfully generated the classic CD key hash" << std::endl;
			else
			{
				std::cout << "Your classic CD key is invalid!" << std::endl;
				return;
			}

			if(hash_d2key(lod_key, client_token, server_token, lod_hash, lod_public))
				std::cout << "Successfully generated the expansion CD key hash" << std::endl;
			else
			{
				std::cout << "Your expansion CD key is invalid!" << std::endl;
				return;
			}

			bool hash_d2key(std::string const & cdkey, ulong client_token, ulong server_token, std::string & output);

			std::string arguments = dword_to_string(client_token) + dword_to_string(0x01000001) + dword_to_string(exe_checksum) + dword_to_string(2) + null + dword_to_string(0x10) + dword_to_string(6) + classic_public + null + classic_hash;
			arguments += dword_to_string(0x10) + dword_to_string(10) + lod_public + null + lod_hash + game_exe_information + zero + key_owner + zero;
			std::string packet = construct_bncs_packet(0x51, arguments);

			//print_data(packet);

			bncs_socket.send(packet);
		}
		else if(type == 0x51)
		{
			ulong result = read_dword(data, 4);
			std::string information = read_one_string(data, 8);
			switch(result)
			{
				case 0x000:
					std::cout << "Successfully logged on to Battle.net" << std::endl;
					break;

				case 0x100:
					std::cout << "Outdated game version" << std::endl;
					break;

				case 0x101:
					std::cout << "Invalid version" << std::endl;
					break;

				case 0x102:
					std::cout << "Game version must be downgraded to " << information << std::endl;
					break;

				case 0x200:
					std::cout << "Invalid CD key" << std::endl;
					break;

				case 0x201:
					std::cout << "CD key is being used by " << information << std::endl;
					break;

				case 0x202:
					std::cout << "This CD key has been banned" << std::endl;
					break;

				case 0x203:
					std::cout << "This CD key is meant to be used with another product" << std::endl;
					break;

				default:
					std::cout << "Failed to log on to Battle.net (" << std::hex << result << ")" << std::endl;
					break;
			}
			if(result == 0)
			{
				std::cout << "Requesting ini file time" << std::endl;
				std::string packet = construct_bncs_packet(0x33, dword_to_string(0x80000004) + null + "bnserver-D2DV.ini" + zero);
				//print_data(packet);
				bncs_socket.send(packet);
			}
			else
				return;
		}
		else if(type == 0x33)
		{
			std::cout << "Logging into the account" << std::endl;
			ulong client_token = nil::time();
			std::string hash = double_hash(client_token, server_token, password);
			std::string packet = construct_bncs_packet(0x3a, dword_to_string(client_token) + dword_to_string(server_token) + hash + account + zero);
			//print_data(packet);
			bncs_socket.send(packet);
		}
		else if(type == 0x3a)
		{
			ulong result = read_dword(data, 4);
			switch(result)
			{
				case 0x00:
					std::cout << "Successfully logged into the account" << std::endl;
					break;

				case 0x01:
					std::cout << "Account does not exist" << std::endl;
					break;

				case 0x02:
					std::cout << "Invalid password specified" << std::endl;
					break;

				case 0x06:
					std::cout << "Account has been closed" << std::endl;
					break;

				default:
					std::cout << "Unknown login error (" << std::hex << result << ")" << std::endl;
					break;
			}

			if(result == 0)
				bncs_socket.send(construct_bncs_packet(0x40, ""));
			else
				return;
		}
		else if(type == 0x40)
		{
			ulong count = read_dword(data, 8);
			std::size_t offset = 12;
			std::cout << "List of realms:" << std::endl;
			for(ulong i = 1; i <= count; i++)
			{
				offset += 4;
				std::string realm_title = read_string(data, offset);
				std::string realm_description = read_string(data, offset);
				std::cout << i << ". " << realm_title << ", " << realm_description << std::endl;

				if(realm.empty() && i == 1)
				{
					std::cout << "No realm was specified in the ini so we're going to connect to " << realm_title << std::endl;
					realm = realm_title;
				}
			}

			std::cout << "Logging on to the realm " << realm << std::endl;

			ulong client_token = 1;
			std::string packet = construct_bncs_packet(0x3e, dword_to_string(client_token) + double_hash(client_token, server_token, "password") + realm + zero);
			//print_data(packet);
			bncs_socket.send(packet);
		}
		else if(type == 0x3e)
		{
			if(data.size() <= 12)
			{
				std::cout << "Failed to log on to realm:" << std::endl;
				print_data(data);
				return;
			}

			//cluster fuck
			//ulong ip = read_nbo_dword(data, 20);
			ulong ip = read_dword(data, 20);
			mcp_port = read_nbo_word(data, 24);
			mcp_ip = nil::net::ip_to_string(ip);
			std::size_t offset = 28;
			mcp_data = data.substr(4, 16) + data.substr(offset, data.size() - offset - 2);

			mcp_thread.start(nil::thread::function_type(*this, &d2_client::mcp_thread_function));
		}
		else if(type == 0x0a)
		{
			std::cout << "Entered the chat" << std::endl;
			std::cout << "Requesting news and advertising data" << std::endl;
			bncs_socket.send(construct_bncs_packet(0x46, null));
			bncs_socket.send(construct_bncs_packet(0x15, platform + lod_id + null + dword_to_string(get_tick_count())));

			game_name = generate_string();
			game_password = generate_string();

			std::cout << "Creating game \"" << game_name << "\" with password \"" << game_password << "\"" << std::endl;

			std::string arguments = word_to_string(game_creation_request_id) + dword_to_string(convert_game_difficulty(difficulty)) + "\x01\x0ff\x08" + game_name + zero + game_password + zero + zero;
			//print_data(arguments);
			mcp_socket.send(construct_mcp_packet(0x03, arguments));

			game_creation_request_id++;
		}
		else if(type == 0x15)
		{
			ulong ad_id = read_dword(data, 4);
			std::cout << "Received advertising data, sending back display confirmation" << std::endl;
			bncs_socket.send(construct_bncs_packet(0x21, platform + lod_id + dword_to_string(ad_id) + zero + zero));
		}
	}
	std::cout << "Disconnected from BNCS" << std::endl;
}

void d2_client::mcp_thread_function(void * unused)
{
	game_creation_request_id = 0x02;

	std::cout << "Connecting to realm server " << mcp_ip << ":" << mcp_port << std::endl;

	if(mcp_socket.connect(mcp_ip, mcp_port))
		std::cout << "Successfully connected to realm server" << std::endl;
	else
		std::cout << "Failed to connect to the realm server" << std::endl;

	mcp_socket.send("\x01");

	std::string packet = construct_mcp_packet(0x01, mcp_data);
	mcp_socket.send(packet);

	std::string mcp_buffer;
	while(true)
	{
		std::string data;
		if(!get_mcp_packet(mcp_socket, mcp_buffer, data))
			break;
		ulong identifier = read_byte(data, 2);

		//std::cout << "MCP data:" << std::endl;
		//std::cout << data << std::endl;
		//print_data(data);

		if(identifier == 0x01)
		{
			ulong result = read_dword(data, 3);
			switch(result)
			{
				case 0x00:
					std::cout << "Successfully logged on to realm server" << std::endl;
					break;

				case 0x7e:
					std::cout << "Your CD key has been banned from playing on this realm!" << std::endl;
					break;

				case 0x7f:
					std::cout << "Your IP has been banned temporarily!" << std::endl;
					break;

				default:
					std::cout << "Unknown realm server logon error occured (" << std::hex << result << ")" << std::endl;
					break;
			}

			if(result != 0)
				return;

			std::cout << "Requesting character list" << std::endl;

			mcp_socket.send(construct_mcp_packet(0x19, dword_to_string(8)));
		}
		else if(identifier == 0x19)
		{
			ulong count = read_word(data, 9);
			if(count == 0)
			{
				std::cout << "There are no characters on this account." << std::endl;
				return;
			}
			else
			{
				bool found_character = false;
				bool selected_first_character = false;
				std::cout << "List of characters on this account:" << std::endl;
				std::size_t offset = 11;
				for(ulong i = 1; i <= count; i++)
				{
					offset += 4;
					std::string character_name = read_string(data, offset);
					std::string stats = read_string(data, offset);
					ulong character_class = read_byte(stats, 13);
					ulong level = read_byte(stats, 25);
					ulong flags = read_byte(stats, 26);
					bool hardcore = (flags & 0x04) != 0;
					bool dead = (flags & 0x08) != 0;
					bool expansion = (flags & 0x20) != 0;
					std::string character_class_string = get_character_class_string(character_class);
					std::string core_string = hardcore ? "Hardcore" : "Softcore";
					std::string expansion_string = expansion ? "Expansion" : "Classic";
					std::cout << i << ". " << character_name << ", Level " << level << " " << character_class_string << " (" << expansion_string << ", " << core_string;
					if(dead)
						std::cout << ", Dead";
					std::cout << ")" << std::endl;
					//print_data(stats);
					if(character.empty() && i == 1)
					{
						selected_first_character = true;
						character = character_name;
					}
					if(character == character_name)
					{
						found_character = true;
						class_byte = character_class - 1;
					}
				}
				if(selected_first_character)
					std::cout << "No character was specified in the ini file so the first character is being used" << std::endl;
				if(!found_character)
				{
					std::cout << "Unable to locate character specified in the ini file";
					return;
				}

				std::cout << "Logging into character " << character << std::endl;

				mcp_socket.send(construct_mcp_packet(0x07, character + zero));
			}
		}
		else if(identifier == 0x07)
		{
			ulong result = read_dword(data, 3);
			if(result != 0)
			{
				std::cout << "Failed to log into character (" << std::hex << result << ")" << std::endl;
				return;
			}
			std::cout << "Successfully logged into character" << std::endl;

			std::cout << "Requesting channel list" << std::endl;
			bncs_socket.send(construct_bncs_packet(0x0b, lod_id));

			std::cout << "Entering the chat server" << std::endl;
			bncs_socket.send(construct_bncs_packet(0x0a, character + zero + realm + "," + character + zero));

			std::cout << "Requesting MOTD" << std::endl;
			mcp_socket.send(construct_mcp_packet(0x12, ""));
		}
		else if(identifier == 0x12)
		{
			std::cout << "Received MOTD" << std::endl;
		}
		else if(identifier == 0x03)
		{
			//print_data(data);

			ulong result = read_dword(data, 9);
			switch(result)
			{
				case 0x00:
					std::cout << "Game has been created successfully" << std::endl;
					break;

				case 0x1e:
					std::cout << "Invalid game name specified" << std::endl;
					break;

				case 0x1f:
					std::cout << "This game already exists" << std::endl;
					break;

				case 0x20:
					std::cout << "Game server down" << std::endl;
					break;

				case 0x6e:
					std::cout << "Your character can't create a game because it's dead!" << std::endl;
					break;
			}

			if(result == 0)
			{
				std::cout << "Joining the game" << std::endl;
				mcp_socket.send(construct_mcp_packet(0x04, word_to_string(game_creation_request_id) + game_name + zero + game_password + zero));
				bncs_socket.send(construct_bncs_packet(0x1c, dword_to_string(1) + null + null + null + null + game_name + zero + game_password + zero + zero));
			}
		}
		else if(identifier == 0x04)
		{
			print_data(data);

			ulong result = read_word(data, 17);

			switch(result)
			{
				case 0x00:
					std::cout << "Successfully joined the game" << std::endl;
					break;

				case 0x29:
					std::cout << "Password is incorrect" << std::endl;
					break;

				case 0x2A:
					std::cout << "Game does not exist" << std::endl;
					break;

				case 0x2B:
					std::cout << "Game is full" << std::endl;
					break;

				case 0x2C:
					std::cout << "You do not meet the level requirements for this game" << std::endl;
					break;

				case 0x71:
					std::cout << "A non-hardcore character cannot join a game created by a Hardcore character." << std::endl;
					break;

				case 0x73:
					std::cout << "Unable to join a Nightmare game." << std::endl;
					break;

				case 0x74:
					std::cout << "Unable to join a Hell game." << std::endl;
					break;

				case 0x78:
					std::cout << "A non-expansion character cannot join a game created by an Expansion character." << std::endl;
					break;

				case 0x79:
					std::cout << "A Expansion character cannot join a game created by a non-expansion character." << std::endl;
					break;

				case 0x7D:
					std::cout << "A non-ladder character cannot join a game created by a Ladder character." << std::endl;
					break;
			}

			if(result == 0)
			{
				ulong ip = read_dword(data, 9);
				gs_ip = nil::net::ip_to_string(ip);
				gs_hash = data.substr(13, 4);
				gs_token = data.substr(5, 2);

				bncs_socket.send(construct_bncs_packet(0x22, lod_id + dword_to_string(0xc) + game_name + zero + game_password + zero));
				bncs_socket.send(construct_bncs_packet(0x10, ""));

				gs_thread.start(nil::thread::function_type(*this, &d2_client::gs_thread_function));
			}
		}
	}
	std::cout << "Disconnected from MCP server" << std::endl;
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
	std::cout << "Connecting to game server " << gs_ip << ":" << gs_port << std::endl;
	if(gs_socket.connect(gs_ip, gs_port))
		std::cout << "Successfully connected to game server" << std::endl;
	else
	{
		std::cout << "Failed to connect to game server" << std::endl;
		return;
	}

	std::string buffer;
	while(true)
	{
		//std::cout << "Waiting for GS data" << std::endl;
		std::string data;
		if(!gs_socket.receive(data))
		{
			std::cout << "Disconnected from game server" << std::endl;
			break;
		}

		//std::cout << "GS data:" << std::endl;
		//print_data(data);

		if(data == "\xaf\x01")
		{
			std::cout << "Logging on to game server" << std::endl;
			std::string packet = "\x68" + gs_hash + gs_token + byte_to_string(class_byte) + dword_to_string(0xc) + "\x50\xcc\x5d\xed\xb6\x19\xa5\x91" + zero + pad_string(character);
			gs_socket.send(packet);
		}
		else
		{
			buffer += data;
			if(buffer.size() < 2 || (static_cast<unsigned char>(buffer[0]) >= 0xF0 && buffer.size() < 3))
			{
				std::cout << "Buffer lacks data to determine packet size" << std::endl;
				//print_data(buffer);
				continue;
			}
			std::size_t header_size;
			std::size_t length = determine_game_packet_size(buffer, header_size);
			length += header_size;
			if(length > buffer.size())
			{
				std::cout << "Buffer lacks data for next length " << header_size << ", " << length << std::endl;
				//print_data(buffer);
				continue;
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
	while(true)
	{
		//std::cout << "Sending ping" << std::endl;
		if(!gs_socket.send("\x6d" + dword_to_string(get_tick_count()) + null + null))
			break;
		nil::sleep(5000);
	}
	std::cout << "Ping thread terminating" << std::endl;
}

void d2_client::process_game_packet(std::string const & packet)
{
	ulong identifier = get_byte(packet, 0);
	if(identifier == 0x00)
		std::cout << "The game is loading, please wait" << std::endl;
	else if(identifier == 0x02)
	{
		std::cout << "The game is done loading, joining the game" << std::endl;
		gs_socket.send("\x6b");
		std::cout << "Launching ping thread" << std::endl;
		ping_thread.start(nil::thread::function_type(*this, &d2_client::ping_thread_function));
	}
	else if(identifier == 0x01)
	{
		//game flags, Diablo II client sends ping
		std::cout << "Replying to game flags with ping" << std::endl;
		gs_socket.send("\x6d" + dword_to_string(get_tick_count()) + dword_to_string(GetTickCount() - last_timestamp) + null);
	}
	else if(identifier == 0x8f)
	{
		//std::cout << "Pong!" << std::endl;
		last_timestamp = get_tick_count();
	}

	//let the derived class handle the packet
	receive_packet(packet);
}
