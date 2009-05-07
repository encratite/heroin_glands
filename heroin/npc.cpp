#include <iostream>
#include <cmath>
#include <heroin/client.hpp>
#include <heroin/bit_reader.hpp>
#include <heroin/utility.hpp>
#include <heroin/game.hpp>
#include <nil/string.hpp>

namespace
{
	std::string const mod_test[] =
	{
		"none",
		"rndname",
		"hpmultiply",
		"light",
		"leveladd",
		"strong",
		"fast",
		"curse",
		"resist",
		"fire",
		"poisondead",
		"durieldead",
		"bloodraven",
		"rage",
		"spcdamage",
		"partydead",
		"champion",
		"lightning",
		"cold",
		"hireable",
		"scarab",
		"killself",
		"questcomplete",
		"poisonhit",
		"thief",
		"manahit",
		"teleport",
		"spectralhit",
		"stoneskin",
		"multishot",
		"aura",
		"goboom",
		"firespike_explode",
		"suicideminion_explode",
		"ai_after_death",
		"shatter_on_death",
		"ghostly",
		"fanatic",
		"possessed",
		"berserk",
		"worms_on_death",
		"always_run_ai",
		"lightningdeath",
	};
}

void d2_client::npc_assignment(std::string const & data)
{
	npc_type output;
	try
	{
		bit_reader reader(data);
		//ac
		reader.read(8);
		ulong id = reader.read(32);
		ulong type = reader.read(16);
		ulong x = reader.read(16);
		ulong y = reader.read(16);
		ulong life = reader.read(8);
		ulong size = reader.read(8);

		output = npc_type(id, type, life, x, y);

		if(debugging)
			std::cout << std::hex << "NPC type: " << type << ", Life: " << life << ", Size: " << data.size() << std::endl;

		std::size_t const information_length = 16;

		string_vector field_entries;
		if(!client_data_manager.monster_fields.get(type, field_entries))
			std::cout << "Failed to read monstats data for NPC of type " << std::hex << type << std::endl;

		if(field_entries.size() != information_length)
			std::cout << "Invalid monstats entry for NPC of type " << std::hex << type << std::endl;

		bool lookup_name = false;

		//experimental
		if(data.size() > 0x10)
		{
			reader.read(4);

			if(reader.read_bool())
			{
				for(ulong i = 0; i < information_length; i++)
				{
					
					ulong bit_count;
					ulong value = nil::string::string_to_number<ulong>(field_entries[i]);
					if(value > 2)
						//equivalent to that bsr thing from the code I stole it from
						bit_count = static_cast<ulong>(std::ceil(std::log(static_cast<double>(value)) / std::log(2.0)));
					else
						bit_count = 1; //?!?
					ulong bits = reader.read(bit_count);
					//std::cout << value << ", " << bit_count << ": " << bits << std::endl;
				}
			}

			if(reader.read_bool())
			{
				output.has_flags = true;

				output.flag1 = reader.read_bool();
				output.flag2 = reader.read_bool();
				output.super_unique = reader.read_bool();
				output.is_minion = reader.read_bool();
				output.flag5 = reader.read_bool();

				//std::cout << flag1 << flag2 << super_unique << is_minion << flag5 << std::endl;
			}

			if(output.type == 0x0201)
			{
				std::cout << "Detected Malah" << std::endl;
				malah_id = output.id;
			}

			if(output.super_unique)
			{
				output.super_unique_id = reader.read(16);
				std::string name;
				if(!client_data_manager.super_uniques.get(output.super_unique_id, name))
					std::cout << "Failed to look up super unique monster name for " << std::hex << output.super_unique_id << std::endl;
				else
				{
					if(debugging)
						std::cout << "Super unique monster detected: " << name << "(" << std::hex << output.super_unique_id << ")" << std::endl;
					output.name = name;
				}
			}
			else
				lookup_name = true;

			while(true)
			{
				ulong mod = reader.read(8);
				if(mod == 0)
					break;
				if(debugging)
					std::cout << "Mod: " << mod_test[mod] << " (" << std::hex << mod << ")" << std::endl;
			}
		}
		else
			lookup_name = true;

		if(lookup_name)
		{
			std::string name;
			if(!client_data_manager.monster_names.get(output.type, name))
				std::cout << "Failed to look up monster name for " << std::hex << output.type << std::endl;
			else
			{
				if(debugging)
					std::cout << "Monster detected: " << name << " (" << std::hex << output.type << ")" << std::endl;
				output.name = name;
			}
		}

		//let derived class process NPC
		new_npc(output);
	}
	catch(nil::exception &)
	{
		std::cout << "Failed to parse 0xac packet" << std::endl;
	}
}
