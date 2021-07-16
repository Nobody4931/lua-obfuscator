#include <cstdint>
#include <random>
#include <utility>
#include <iostream>
#include <fstream>

#include "bytecode/deserializer.hpp"
#include "bytecode/datatypes.hpp"
#include "bytecode/instruction.hpp"
#include "bytecode/constant.hpp"
#include "bytecode/chunk.hpp"

void decode_constants( chunk_t& chunk, std::ifstream& stream, bool little_endian ) {
	chunk.constant_cnt = read_int32( stream, little_endian );

	if ( chunk.constant_cnt == 0 ) {
		chunk.constants = nullptr;
		return;
	}

	chunk.constants = new constant_t[ chunk.constant_cnt ];
	for ( l_int i = 0; i < chunk.constant_cnt; ++i ) {
		const_t type = static_cast<const_t>( read_byte( stream ) );

		chunk.constants[i] = {
			.type = type
		};

		// ALSO WHAT THE FUCK LUA
		switch ( type ) {

			case const_t::K_NIL: // shut up compiler
				chunk.constants[i].data = nullptr;
				break;

			case const_t::K_BOOLEAN:
				chunk.constants[i].data =
						new uint8_t( read_byte( stream ) );
				break;

			case const_t::K_NUMBER:
				chunk.constants[i].data = reinterpret_cast<uint8_t*>(
						new l_number( read_double( stream, little_endian ) ) );
				break;

			case const_t::K_STRING:
				chunk.constants[i].data = reinterpret_cast<uint8_t*>(
						new l_string( read_string( stream, little_endian ) ) );
				break;

		}
	}
}


void shuffle_constants( chunk_t& chunk, std::default_random_engine& rand_engine ) {
	l_int shuffle_map[ chunk.constant_cnt ];
	for ( l_int i = 0; i < chunk.constant_cnt; ++i )
		shuffle_map[i] = i;

	for ( l_int i = 0; i < chunk.constant_cnt; ++i )
		std::swap( shuffle_map[ i ], shuffle_map[ rand_engine() % ( i + 1 ) ] );

	for ( l_int i = 0; i < chunk.instruction_cnt; ++i ) {
		instruction_t& instruction = chunk.instructions[i];

		// thanks lua for making so many bullshit instruction field rules!
		switch ( instruction.opcode ) {

			case OP_LOADK:
			case OP_GETGLOBAL:
			case OP_SETGLOBAL:
				instruction.b = shuffle_map[ instruction.b ];
				break;

			case OP_GETTABLE:
			case OP_SELF:
				if ( instruction.c & 256 )
					instruction.c = shuffle_map[ instruction.c ^ 256 ] | 256;
				break;

			case OP_SETTABLE:
			case OP_ADD:
			case OP_SUB:
			case OP_MUL:
			case OP_DIV:
			case OP_MOD:
			case OP_POW:
			case OP_EQ:
			case OP_LT:
			case OP_LE:
				if ( instruction.b & 256 )
					instruction.b = shuffle_map[ instruction.b ^ 256 ] | 256;
				if ( instruction.c & 256 )
					instruction.c = shuffle_map[ instruction.c ^ 256 ] | 256;
				break;

			default: // shut up compiler
				break;

		}
	}

	for ( l_int i = 0; i < chunk.constant_cnt; ++i ) {
		while ( shuffle_map[i] != i ) {
			std::swap( chunk.constants[ i ], chunk.constants[ shuffle_map[i] ] );
			std::swap( shuffle_map[ i ], shuffle_map[ shuffle_map[i] ] );
		}
	}
}
