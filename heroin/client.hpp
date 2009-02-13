#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#include <nil/net.hpp>
#include <nil/thread.hpp>

#include "game.hpp"

enum d2_client_status
{
	d2_client_status_idle,
	d2_client_status_active,
	d2_client_status_terminating
};

class d2_client
{
public:
	d2_client();

	void set_binary_information(std::string const & new_d2_location, std::string const & new_game_exe_information);
	void set_realm_information(std::string const & new_battle_net_server, std::string const & new_realm);
	void set_key_data(std::string const & new_classic_key, std::string const & new_lod_key, std::string const & new_key_owner);
	void set_login_data(std::string const & new_account, std::string const & new_password, std::string const & new_character);
	void set_difficulty(std::string const & new_difficulty);

	void launch();
	void terminate();

protected:

	d2_client_status status;
	nil::thread bncs_thread, mcp_thread, gs_thread, ping_thread;
	nil::net::tcp_socket bncs_socket, mcp_socket, gs_socket;

	std::string
		d2_location,
		game_exe_information,
		battle_net_server,
		realm,
		classic_key,
		lod_key,
		key_owner,
		account,
		password,
		character;

	std::string game_name, game_password;

	std::string mcp_data, mcp_ip;
	ulong mcp_port;

	std::string gs_ip, gs_hash, gs_token;
	ulong class_byte;

	ulong game_creation_request_id;

	game_difficulty_type difficulty;
	ulong last_timestamp;

	virtual void receive_packet(std::string const & packet);
	void send_packet(std::string const & packet);

private:

	void bncs_thread_function(void * unused);
	void mcp_thread_function(void * unused);
	void gs_thread_function(void * unused);
	void ping_thread_function(void * unused);
	void process_game_packet(std::string const & packet);
};

#endif
