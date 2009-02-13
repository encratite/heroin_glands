#include <iostream>

#include <nil/ini.hpp>

#include <heroin/client.hpp>
#include <heroin/utility.hpp>

//to use the D2 client you derive a class from it
class test_client: public d2_client
{
public:
	test_client(data_manager & manager):
		d2_client(manager)
	{
	}

protected:

	//override receive_packet to read the stuff
	void receive_packet(std::string const & packet)
	{
		//this program simply pings the server (handled by d2_client), idles in a game
		//uncomment the next line if you want to see the packet spam you get from the game server (not recommended on first run)
		//print_data(packet);
	}
};

int main(int argc, char ** argv)
{
	if(argc != 2)
	{
		std::cout << argv[0] << " <ini>" << std::endl;
		return 1;
	}

	nil::ini ini;
	if(!ini.load(argv[1]))
	{
		std::cout << "Unable to load ini file from specified directory." << std::endl;
		return 1;
	}

	try
	{
		//load relevant data from the ini file specified in the command line, example content of file:

		/*
		#for "Game.exe", "Bnclient.dll", "D2Client.dll":
		d2_binary_directory = "C:\d2\"
		#text_data_directory is the path to the heroin_glands data .txt files
		text_data_directory = "D:\Code\heroin_glands\heroin\data\"
		game_exe_information = "Game.exe 08/03/08 19:03:34 61440"
		battle_net_server = "europe.battle.net"
		classic_key = "AAAAAAAAAAAAAAAA"
		lod_key = "BBBBBBBBBBBBBBBB"
		key_owner = "Owner"
		account = "Account"
		password = "Password"
		#valid difficulties are: Normal, Nightmare, Hell (upper/lowercase doesn't matter)
		difficulty = "Normal"
		*/

		std::string
			d2_binary_directory = ini.string("d2_binary_directory"),
			text_data_directory = ini.string("text_data_directory"),
			game_exe_information = ini.string("game_exe_information"),
			battle_net_server = ini.string("battle_net_server"),
			realm = ini.string("realm", ""), //only relevant for Asia multi realm servers
			classic_key = ini.string("classic_key"),
			lod_key = ini.string("lod_key"),
			key_owner = ini.string("key_owner"),
			account = ini.string("account"),
			password = ini.string("password"),
			character = ini.string("character", ""),
			difficulty = ini.string("difficulty", "normal");

		//the data manager handles the heroin_glands data .txt files and the Diablo II .bin files
		//it is stored independently from the client objects because one manager can be used by all clients
		data_manager manager(text_data_directory);

		test_client client(manager);
		client.debugging = true;
		client.set_binary_information(d2_binary_directory, game_exe_information);
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
