#pragma once
#ifndef DESERIALIZER_HEADER_HPP
#define DESERIALIZER_HEADER_HPP

#include <iostream>
#include <fstream>

#include "deserializer/chunk.hpp"


chunk_t decode_file( std::ifstream& stream );
chunk_t decode_chunk( std::ifstream& stream, bool little_endian );

void decode_instructions( chunk_t& chunk, std::ifstream& stream, bool little_endian );
void decode_constants( chunk_t& chunk, std::ifstream& stream, bool little_endian );

#endif
