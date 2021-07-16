#pragma once
#ifndef DESERIALIZER_HEADER_HPP
#define DESERIALIZER_HEADER_HPP

#include <random>
#include <iostream>
#include <fstream>

#include "bytecode/chunk.hpp"
#include "bytecode/instruction.hpp"


chunk_t decode_file( std::ifstream& stream );
chunk_t decode_chunk( std::ifstream& stream, bool little_endian );

void decode_instructions( chunk_t& chunk, std::ifstream& stream, bool little_endian );
void decode_constants( chunk_t& chunk, std::ifstream& stream, bool little_endian );

void shuffle_constants( chunk_t& chunk, std::default_random_engine& rand_engine );
void shuffle_functions( chunk_t& chunk, std::default_random_engine& rand_engine );

#endif
