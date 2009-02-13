#include <iostream>
#include <nil/environment.hpp>
#include <nil/array.hpp>
#include <heroin/check_revision.hpp>
#include <heroin/d2cdkey.hpp>
#include <heroin/bsha1.hpp>
#include <heroin/utility.hpp>

std::string result_to_string(check_revision_result_type result)
{
	switch(result)
	{
		case check_revision_result_success:
			return "Success";
		case check_revision_result_formula_error:
			return "Formula error";
		case check_revision_result_mpq_error:
			return "MPQ error";
		case check_revision_result_file_error:
			return "File error";
	}
	return "Unknown error";
}

namespace
{
	//all authentic

	std::string const keys[] =
	{
		"MKK46R7NC48M6PTV",
		"4CKEV7BZD7RXMBEN",
		"6BB4CJKJDK7GDWRG",
		"647Z62BXJHRJRRJD",
		"NRRXVRK722J2K7PB",
		"KPJKF7BX6Z6KCRW4",
		"JT6C9FKM8TKB9XG8",
		"8W4NTGBWGPNT2EG4"
	};
	std::string const mpq_strings[] =
	{
		"ver-IX86-6.mpq",
		"ver-IX86-5.mpq",
		"ver-IX86-4.mpq",
	};


	std::string const formulas[] =
	{
		"C=1588081752 B=71666965 A=3083503560 4 A=A+S B=B-C C=C^A A=A+B",
		"A=3303665863 C=2206781070 B=2003605322 4 A=A^S B=B^C C=C+A A=A^B",
		"C=4148468687 B=3106478836 A=3512970683 4 A=A+S B=B^C C=C+A A=A^B"
	};

	ulong const client_token = 0x00491244;
	ulong const server_token = 0xb8997ed1;

	std::string const passwords[] =
	{
		"abc123",
		"qwerty",
		"heilhitler",
		"opensource",
		"diablo2",
		"password"
	};

	ulong correct_check_revision_sums[] =
	{
		0x3511822d,
		0x29fb75c8,
		0x5b9f0146
	};
}

int main(int argc, char ** argv)
{
	if(argc != 2)
	{
		std::cout << argv[0] << " <path to D2 binaries with terminating slash/backslash>" << std::endl;
		return 1;
	}

	std::string directory(argv[1]);

	std::cout << "Running check_revision tests:" << std::endl << std::endl;

	for(std::size_t i = 0; i < nil::countof(mpq_strings); i++)
	{
		ulong sum = 0;
		check_revision_result_type result = check_revision(formulas[i], mpq_strings[i], directory, sum);
		std::cout << "Key " << (i + 1) << " result: " << result_to_string(result) << ", checksum: " << std::hex << sum << std::endl;
	}

	std::cout << std::endl << "Running hash_d2key tests:" << std::endl << std::endl;

	for(std::size_t i = 0; i < nil::countof(keys); i++)
	{
		std::string hash, public_value;
		bool result = hash_d2key(keys[i], client_token, server_token, hash, public_value);
		std::cout << "Key " << (i + 1);
		if(result)
		{
			std::cout << ":" << std::endl;
			std::cout << "Hash: ";
			print_data(hash);
			std::cout << "Public value: ";
			print_data(public_value);
		}
		else
			std::cout << " is invalid" << std::endl;
	}

	std::cout << std::endl << "Running double_hash tests:" << std::endl << std::endl;

	for(std::size_t i = 0; i < nil::countof(passwords); i++)
	{
		std::string hash = double_hash(client_token, server_token, passwords[i]);
		std::cout << "Password " << (i + 1) << ": ";
		print_data(hash);
	}

#ifdef NIL_WINDOWS
	//haha
	std::cin.get();
#endif

	return 0;
}
