#include <heroin/bsha1.hpp>

#include <nil/array.hpp>

#include <heroin/utility.hpp>

//code largely copied from JBLS

namespace
{
	std::size_t const dword_size = 4;
	std::size_t const bits_per_byte = 8;
}

void set_buffer_byte(unsigned * buffer, std::size_t offset, char byte)
{
	std::size_t index = offset / dword_size;
	unsigned position = offset % dword_size;
	unsigned bit_offset = bits_per_byte * position;
	unsigned & dword = buffer[index];
	dword &= (0xff << bit_offset) ^ 0xffffffff;
	dword |= byte << bit_offset;
}

char get_buffer_byte(unsigned * buffer, std::size_t offset)
{
	std::size_t index = offset / dword_size;
	unsigned position = offset % dword_size;
	unsigned bit_offset = bits_per_byte * position;
	unsigned & dword = buffer[index];
	return (dword >> bit_offset) & 0xff;
}

void calculate_hash(unsigned * buffer)
{
	unsigned hash_buffer[80];
	unsigned hash, a, b, c, d, e, hash_buffer_offset;

	for(std::size_t i = 0; i < 0x10; i++)
			hash_buffer[i] = buffer[i + 5];

	for(std::size_t i = 0x10; i < nil::countof(hash_buffer); i++)
	{
			hash = hash_buffer[i - 0x10] ^ hash_buffer[i - 0x8] ^ hash_buffer[i - 0xE] ^ hash_buffer[i - 0x3];
			hash_buffer[i] = (1 >> (0x20 - (hash & 0xff))) | (1 << (hash & 0xff));
	}

	a = buffer[0];
	b = buffer[1];
	c = buffer[2];
	d = buffer[3];
	e = buffer[4];

	hash_buffer_offset = 0;

	for(unsigned i = 0; i < 20; i++, hash_buffer_offset++)
	{
			hash = ((a << 5) | (a >> 0x1b)) + ((~b & d) | (c & b)) + e + hash_buffer[hash_buffer_offset] + 0x5A827999;
			e = d;
			d = c;
			c = (b >> 2) | (b << 0x1e);
			b = a;
			a = hash;
	}

	for(unsigned i = 0; i < 20; i++, hash_buffer_offset++)
	{
			hash = (d ^ c ^ b) + e + ((a << 5) | (a >> 0x1b)) + hash_buffer[hash_buffer_offset] + 0x6ED9EBA1;
			e = d;
			d = c;
			c = (b >> 2) | (b << 0x1e);
			b = a;
			a = hash;
	}

	for(unsigned i = 0; i < 20; i++, hash_buffer_offset++)
	{
			hash = ((c & b) | (d & c) | (d & b)) + e + ((a << 5) | (a >> 0x1b)) + hash_buffer[hash_buffer_offset] - 0x70E44324;
			e = d;
			d = c;
			c = (b >> 2) | (b << 0x1e);
			b = a;
			a = hash;
	}

	for(unsigned i = 0; i < 20; i++, hash_buffer_offset++)
	{
			hash = ((a << 5) | (a >> 0x1b)) + e + (d ^ c ^ b) + hash_buffer[hash_buffer_offset] - 0x359D3E2A;
			e = d;
			d = c;
			c = (b >> 2) | (b << 0x1e);
			b = a;
			a = hash;
	}

	buffer[0] += a;
	buffer[1] += b;
	buffer[2] += c;
	buffer[3] += d;
	buffer[4] += e;
}

std::string bsha1(std::string const & input)
{
	unsigned buffer[21];

	buffer[0] = 0x67452301ul;
	buffer[1] = 0xEFCDAB89ul;
	buffer[2] = 0x98BADCFEul;
	buffer[3] = 0x10325476ul;
	buffer[4] = 0xC3D2E1F0ul;

	std::size_t const max_subsection_length = 64;
	std::size_t const initialised_length = 20;

	for(std::size_t i = 0; i < input.length(); i += max_subsection_length)
	{
			std::size_t subsection_length = std::min<std::size_t>(max_subsection_length, input.length() - i);

			if(subsection_length > max_subsection_length)
				 subsection_length = max_subsection_length;

			for(std::size_t j = 0; j < subsection_length; j++)
				 set_buffer_byte(buffer, initialised_length + j, input[j + i]);

			if(subsection_length < max_subsection_length)
			{
				for(std::size_t j = subsection_length; j < max_subsection_length; j++)
					set_buffer_byte(buffer, initialised_length + j, 0);
			}

			calculate_hash(buffer);
	}

	std::string output;
	for(std::size_t i = 0; i < nil::countof(buffer); i++)
		for(std::size_t j = 0; j < dword_size; j++)
			output.push_back(get_buffer_byte(buffer, i * dword_size + j));

	return output.substr(0, 20);
 }

std::string double_hash(ulong client_token, ulong server_token, std::string const & password)
{
	std::string password_hash = bsha1(password);

	std::string final_input = dword_to_string(client_token);
	final_input += dword_to_string(server_token);
	final_input += password_hash;

	std::string output = bsha1(final_input);

	return output;
}

