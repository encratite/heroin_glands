#ifndef COMPRESSION_HPP
#define COMPRESSION_HPP

std::size_t determine_game_packet_size(std::string const & input, std::size_t & header_size);
std::string decompress_packet(std::string const & input);
std::string compress_packet(std::string const & input);

#endif
