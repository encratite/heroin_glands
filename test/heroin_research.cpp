#include <iostream>
#include <cmath>

#include <nil/ini.hpp>
#include <nil/array.hpp>

#include <heroin/client.hpp>
#include <heroin/game.hpp>
#include <heroin/utility.hpp>

namespace
{
	coordinate const a5_town_coordinates[] =
	{
		coordinate(0x13e1, 0x13a7),
		coordinate(0x13d3, 0x13a8),
		coordinate(0x13d4, 0x13bf),
		coordinate(0x13db, 0x13da),
		coordinate(0x13dc, 0x13f4),
		coordinate(0x13f0, 0x1402),
		coordinate(0x13fa, 0x1400)
	};
}

class research_client: public d2_client
{
public:
	research_client(data_manager & manager):
		d2_client(manager)
	{
	}

	void print_containers() const
	{
		inventory.print_fields();
		stash.print_fields();
	}

	void run_item_test(std::string const & input)
	{
		item_action(input);
	}

	void run_container_test() const
	{
		item_type item;
		item.width = 2;
		item.height = 3;

		coordinate location;

		if(stash.find_free_space(item, location))
			std::cout << "Position in stash: " << location.string() << std::endl;
		else
			std::cout << "Failed to determine position in stash" << std::endl;

		if(inventory.find_free_space(item, location))
			std::cout << "Position in inventory: " << location.string() << std::endl;
		else
			std::cout << "Failed to determine position in inventory" << std::endl;

		item.width = 4;
		item.height = 1;

		if(inventory.find_free_space(item, location))
			std::cout << "Huge item position in inventory: " << location.string() << std::endl;
		else
			std::cout << "Failed to determine huge item position in inventory" << std::endl;
	}

protected:

	void receive_packet(std::string const & packet)
	{
		//print_data(packet);
	}
};

ulong test(ulong value)
{
	if(value <= 2)
		return 1;
	ulong bit_count = static_cast<ulong>(std::ceil(std::log(static_cast<double>(value)) / std::log(2.0)));
	return bit_count;
}

ulong bsr(ulong value)
{
	ulong output;
	__asm
	{
		bsr eax, value;
		mov output, eax;
	}
	return output;
}

ulong test2(ulong value)
{
	BYTE bitCount = 1;
 
	if (value >= 3)
	{
		bitCount = bsr(value - 1) + 1;//number to bit count needed to represent it.(i.e. 7 need 3 bits)
	}
	else bitCount = 1;
	return bitCount;
}

void run_npc_tests(research_client & client)
{
	unsigned char const minion1[] = {0xac, 0xa4, 0x90, 0xaf, 0x6c, 0xb8, 0x01, 0x4c, 0x27, 0x9c, 0x33, 0x80, 0x17, 0x91, 0x02, 0x20, 0x12, 0x49, 0xd8, 0x00, 0x00, 0x00, 0x00};
	unsigned char const minion2[] = {0xac, 0x48, 0x21, 0x5f, 0xd9, 0xb8, 0x01, 0x4d, 0x27, 0xa2, 0x33, 0x80, 0x17, 0x91, 0x00, 0x20, 0x12, 0x49, 0xd8, 0x00, 0x00, 0x00, 0x00};
	unsigned char const minion3[] = {0xac, 0x91, 0x82, 0xbe, 0x92, 0xb8, 0x01, 0x4d, 0x27, 0xa0, 0x33, 0x80, 0x17, 0x91, 0x02, 0x20, 0x12, 0x49, 0xd8, 0x00, 0x00, 0x00, 0x00};
	unsigned char const pindle1[] = {0xac, 0x23, 0x05, 0x7d, 0x05, 0xb8, 0x01, 0x4a, 0x27, 0x9f, 0x33, 0x80, 0x1b, 0xd1, 0x02, 0xa0, 0xa1, 0x01, 0x10, 0x49, 0xd8, 0xf0, 0xb0, 0x00, 0x90, 0x12, 0x04};
	unsigned char const pindle2[] = {0xac, 0x0a, 0x0a, 0x68, 0x42, 0xb8, 0x01, 0x4a, 0x27, 0x9f, 0x33, 0x80, 0x1b, 0xf1, 0x01, 0xa0, 0xa1, 0x01, 0x10, 0x49, 0x88, 0xd0, 0xb0, 0x00, 0x48, 0xb2, 0x01};

	//Fire enchanted, Cursed - Immune to poison 
	unsigned char const pindle4[] = {0xAC, 0x08, 0x1A, 0xC0, 0xAD, 0xB8, 0x01, 0x4F, 0x27, 0xB2, 0x33, 0x80, 0x1A, 0xF1, 0x60, 0xA0, 0xA1, 0x01, 0x10, 0x49, 0x38, 0xB0, 0x00, 0x68, 0xD9, 0x03};

	//Fire enchanted, Cursed - Immune to poison 
	unsigned char const pindle5[] = {0xAC, 0x13, 0xA8, 0xAF, 0x27, 0xB8, 0x01, 0x4B, 0x27, 0xB9, 0x33, 0x80, 0x1A, 0x91, 0x02, 0xA0, 0xA1, 0x01, 0x10, 0x49, 0x38, 0xB0, 0x00, 0x90, 0xF4, 0x02};

	//Fire enchanted, Magic Resistance - Immune to poison, Immune to fire 
	unsigned char const pindle6[] = {0xAC, 0x85, 0x33, 0xCD, 0x2E, 0xB8, 0x01, 0x46, 0x27, 0xA1, 0x33, 0x80, 0x1A, 0xF1, 0x21, 0xA0, 0xA1, 0x01, 0x10, 0x49, 0x40, 0xB0, 0x00, 0x60, 0xAD, 0x00};

	//Fire enchanted, Magic Resistance - Immune to poison, Immune to fire 
	unsigned char const pindle7[] = {0xAC, 0x98, 0xB4, 0x74, 0xA7, 0xB8, 0x01, 0x50, 0x27, 0xC6, 0x33, 0x80, 0x1A, 0x31, 0x63, 0xA0, 0xA1, 0x01, 0x10, 0x49, 0x40, 0xB0, 0x00, 0xE0, 0xF8, 0x05};

	//Fire enchanted, Extra Strong - Immune to Poison 
	unsigned char const pindle8[] = {0xAC, 0x44, 0x1F, 0x05, 0x94, 0xB8, 0x01, 0x4D, 0x27, 0xC9, 0x33, 0x80, 0x1A, 0x71, 0x63, 0xA0, 0xA1, 0x01, 0x10, 0x49, 0x28, 0xB0, 0x00, 0x88, 0x13, 0x02};

	//Fire enchanted, Teleportation, Aura enchanted - Immune to poison 
	unsigned char const pindle9[] = {0xAC, 0xF0, 0x12, 0x73, 0x76, 0xB8, 0x01, 0x4E, 0x27, 0xBB, 0x33, 0x2E, 0x1B, 0xD1, 0x01, 0xA0, 0xA1, 0x01, 0x10, 0x49, 0xD0, 0xF0, 0xB0, 0x00, 0x98, 0xCD, 0x06};
	//- The aura is not included in this packet but with 0xA8 

	//Fire enchanted, Cold Enchanted, Special hit - Immune to poison, cold 
	unsigned char const pindle10[] = {0xAC, 0xE4, 0x25, 0x39, 0x45, 0xB8, 0x01, 0x48, 0x27, 0xAD, 0x33, 0x7F, 0x1B, 0x51, 0x22, 0xA0, 0xA1, 0x01, 0x10, 0x49, 0xD8, 0x90, 0xB0, 0x00, 0x10, 0xB0, 0x06};

	//Fire enchanted, Cursed, Magic Resistance - Immune to fire/poison 
	unsigned char const pindle11[] = {0xAC, 0xDC, 0x15, 0xF1, 0x29, 0xB8, 0x01, 0x45, 0x27, 0xB7, 0x33, 0x7F, 0x1B, 0x51, 0x23, 0xA0, 0xA1, 0x01, 0x10, 0x49, 0x38, 0x40, 0xB0, 0x00, 0xD8, 0x16, 0x01};

	//Fire enchanted, Stone skin, Aura enchanted - Immune to Physical/poison 
	unsigned char const pindle12[] = {0xAC, 0xEC, 0xB3, 0x0E, 0x85, 0xB8, 0x01, 0x4D, 0x27, 0xB3, 0x33, 0x80, 0x1B, 0x91, 0x20, 0xA0, 0xA1, 0x01, 0x10, 0x49, 0xE0, 0xF0, 0xB0, 0x00, 0xD0, 0xF4, 0x02};

	client.npc_assignment(std::string((char const *)minion1, sizeof(minion1)));
	client.npc_assignment(std::string((char const *)minion2, sizeof(minion2)));
	client.npc_assignment(std::string((char const *)minion3, sizeof(minion3)));
	client.npc_assignment(std::string((char const *)pindle1, sizeof(pindle1)));
	client.npc_assignment(std::string((char const *)pindle2, sizeof(pindle2)));

	std::cout << "Fire enchanted, Cursed - Immune to poison:" << std::endl;
	client.npc_assignment(std::string((char const *)pindle4, sizeof(pindle4)));

	std::cout << "Fire enchanted, Cursed - Immune to poison:" << std::endl;
	client.npc_assignment(std::string((char const *)pindle5, sizeof(pindle5)));

	std::cout << "Fire enchanted, Magic Resistance - Immune to poison, Immune to fire:" << std::endl;
	client.npc_assignment(std::string((char const *)pindle6, sizeof(pindle6)));

	std::cout << "Fire enchanted, Magic Resistance - Immune to poison, Immune to fire:" << std::endl;
	client.npc_assignment(std::string((char const *)pindle7, sizeof(pindle7)));

	std::cout << "Fire enchanted, Extra Strong - Immune to Poison:" << std::endl;
	client.npc_assignment(std::string((char const *)pindle8, sizeof(pindle8)));

	std::cout << "Fire enchanted, Teleportation, Aura enchanted - Immune to poison:" << std::endl;
	client.npc_assignment(std::string((char const *)pindle9, sizeof(pindle9)));

	std::cout << "Fire enchanted, Cold Enchanted, Special hit - Immune to poison, cold:" << std::endl;
	client.npc_assignment(std::string((char const *)pindle10, sizeof(pindle10)));

	std::cout << "Fire enchanted, Cursed, Magic Resistance - Immune to fire/poison" << std::endl;
	client.npc_assignment(std::string((char const *)pindle11, sizeof(pindle11)));

	std::cout << "Fire enchanted, Stone skin, Aura enchanted - Immune to Physical/poison" << std::endl;
	client.npc_assignment(std::string((char const *)pindle12, sizeof(pindle12)));
}

void run_item_tests(research_client & client)
{
	unsigned char const rare_ring[] = {0x9d, 0x06, 0x37, 0x10, 0xc6, 0xb0, 0xaf, 0x6d, 0x00, 0x60, 0xa3, 0x19, 0x66, 0x11, 0x00, 0x80, 0x00, 0x65, 0xe4, 0x0e, 0x20, 0x97, 0xe6, 0x06, 0x02, 0xac, 0x95, 0xcf, 0xbe, 0x19, 0xb9, 0xae, 0x4e, 0xd9, 0x17, 0x57, 0x02, 0xe0, 0x25, 0x80, 0xe8, 0xc4, 0x4e, 0x8a, 0x9d, 0x15, 0x3b, 0x2d, 0x76, 0x7c, 0x18, 0xa0, 0xb4, 0xfd, 0x07};
	unsigned char const set_amulet[] = {0x9d, 0x06, 0x2d, 0x10, 0x4d, 0x0d, 0x9b, 0x26, 0x00, 0x69, 0xaa, 0xd8, 0x14, 0x11, 0x00, 0x80, 0x08, 0x65, 0x44, 0x04, 0x10, 0xd6, 0x56, 0x07, 0x02, 0x66, 0x8d, 0x26, 0x20, 0x07, 0xa4, 0x24, 0x50, 0x4a, 0x31, 0x25, 0x63, 0x00, 0x61, 0x4a, 0xf4, 0x3f, 0x8d, 0xe7, 0x3f};
	unsigned char const talons[] = {0x9c, 0x04, 0x25, 0x05, 0x61, 0x94,0x27, 0x51, 0x10, 0x08, 0x80, 0x08, 0x65, 0x00, 0x04, 0x9a, 0x43, 0x77, 0x07, 0x02, 0x2f, 0xe1, 0x20, 0x53, 0x14, 0x15, 0x89, 0x2e, 0x9e, 0x35, 0x06, 0xa7, 0x8d, 0x00, 0x17, 0xfe, 0x07};

	//client.run_item_test(std::string(reinterpret_cast<char const *>(rare_ring), sizeof(rare_ring)));
	//client.run_item_test(std::string(reinterpret_cast<char const *>(set_amulet), sizeof(set_amulet)));
	client.run_item_test(std::string(reinterpret_cast<char const *>(talons), sizeof(talons)));
}

int main(int argc, char ** argv)
{
	/*
	for(ulong i = 0; i < 10; i++)
		std::cout << i << ": " << test(i) << " " << test2(i) << std::endl;
	std::cin.get();
	return 0;
	*/

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
			d2_binary_directory = ini.string("d2_binary_directory"),
			text_data_directory = ini.string("text_data_directory"),
			game_exe_information = ini.string("game_exe_information"),
			battle_net_server = ini.string("battle_net_server"),
			realm = ini.string("realm", ""),
			classic_key = ini.string("classic_key"),
			lod_key = ini.string("lod_key"),
			key_owner = ini.string("key_owner"),
			account = ini.string("account"),
			password = ini.string("password"),
			character = ini.string("character", ""),
			difficulty = ini.string("difficulty", "normal"),
			game_name = ini.string("game_name", ""),
			game_password = ini.string("game_password", "");

		data_manager manager(text_data_directory);

		research_client client(manager);
		client.debugging = true;
		client.set_binary_information(d2_binary_directory, game_exe_information);
		client.set_realm_information(battle_net_server, realm);
		client.set_key_data(classic_key, lod_key, key_owner);
		client.set_login_data(account, password, character);
		client.set_difficulty(difficulty);

		client.set_game_data(game_name.empty(), game_name, game_password);	

		run_item_tests(client);
		std::cin.get();
		return 0;

		client.launch();
		while(true)
		{
			std::cout << ">> ";
			std::string line;
			std::getline(std::cin, line);
			if(line.empty())
				continue;
			std::vector<std::string> tokens = nil::string::tokenise(line, ' ');
			std::string const & command = tokens[0];
			if(command == "go")
			{
				std::size_t index;
				if(!nil::string::string_to_number(tokens[1], index))
				{
					std::cout << "Invalid coordinate specified" << std::endl;
					continue;
				}
				else if(index < 0 || index >= nil::countof(a5_town_coordinates))
				{
					std::cout << "Invalid index, out of range" << std::endl;
					continue;
				}
				coordinate const & target = a5_town_coordinates[index];
				std::cout << "Going to location " << index << ": " << target.string() <<  std::endl;
				client.move_to(target);
			}
			else if(command == "full")
			{
				std::cout << "Performing full run" << std::endl;
				for(std::size_t i = 0; i < nil::countof(a5_town_coordinates); i++)
				{
					std::cout << "Processing " << i << std::endl;
					client.move_to(a5_town_coordinates[i]);
				}
			}
			else if(command == "container")
				client.print_containers();
			else if(line == "run test")
				client.run_container_test();
			else if(line == "quit")
			{
				client.leave_game();
				std::cout << "leave_game returned" << std::endl;
			}
			else if(line == "kill")
				client.terminate();
		}
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
