#include <iostream>

#include <nil/ini.hpp>

#include "client.hpp"

#include "compression.hpp"
#include "utility.hpp"

unsigned char data[] = {0x07, 0x1f, 0x7f, 0xff, 0xff, 0xff, 0xc0};

void process_compressed_data(unsigned char * input, std::size_t size)
{
	std::string data(reinterpret_cast<char *>(input), size);
	std::size_t header_size;
	std::cout << determine_game_packet_size(data, header_size) << " " << size << std::endl;
	std::cout << "Header size " << header_size << std::endl;
	print_data(decompress_packet(data));
}

int main(int argc, char ** argv)
{
	//process_compressed_data(data, sizeof(data));
	//std::cin.get();

	if(argc != 2)
	{
		std::cout << argv[0] << " <ini>" << std::endl;
		return 1;
	}

	nil::ini ini;
	if(!ini.load(argv[1]))
	{
		std::cout << "Unable to load ini file from specified location." << std::endl;
		return 1;
	}

	try
	{
		std::string
			d2_location = ini.string("d2_location"),
			game_exe_information = ini.string("game_exe_information"),
			battle_net_server = ini.string("battle_net_server"),
			realm = ini.string("realm", ""),
			classic_key = ini.string("classic_key"),
			lod_key = ini.string("lod_key"),
			key_owner = ini.string("key_owner"),
			account = ini.string("account"),
			password = ini.string("password"),
			character = ini.string("character", ""),
			difficulty = ini.string("difficulty", "normal");

		d2_client client;
		client.set_binary_information(d2_location, game_exe_information);
		client.set_realm_information(battle_net_server, realm);
		client.set_key_data(classic_key, lod_key, key_owner);
		client.set_login_data(account, password, character);
		client.set_difficulty(difficulty);

		client.launch();
		nil::sleep();
	}
	catch(nil::exception & exception)
	{
		std::cout << "nil exception: " << exception.get_message() << std::endl;
	}
	catch(std::exception & exception)
	{
		std::cout << "std exception: " << exception.what() << std::endl;
	}

	std::cin.get();
	return 0;
}
