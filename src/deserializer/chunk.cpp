#include <random>
#include <ctime>
#include <iostream>
#include <fstream>

#include "deserializer/deserializer.hpp"
#include "deserializer/datatypes.hpp"
#include "deserializer/instruction.hpp"
#include "deserializer/chunk.hpp"

chunk_t decode_chunk( std::ifstream& stream, bool little_endian ) {
	chunk_t chunk {
		.name = read_string( stream, little_endian ),
		.line = read_int32( stream, little_endian ),
		.line_l = read_int32( stream, little_endian ),

		.upvalue_cnt = read_byte( stream ),
		.parameter_cnt = read_byte( stream ),
		.vararg_flag = read_byte( stream ),
		.stack_max = read_byte( stream )
	};

	decode_instructions( chunk, stream, little_endian );
	decode_constants( chunk, stream, little_endian );

	// not making another function for this fuck off
	chunk.function_cnt = read_int32( stream, little_endian );
	chunk.functions = nullptr; // readability or efficiency?

	if ( chunk.function_cnt > 0 ) {
		chunk.functions = new chunk_t[ chunk.function_cnt ];
		for ( l_int i = 0; i < chunk.function_cnt; ++i ) {
			chunk.functions[i] = decode_chunk( stream, little_endian );
		}
	}

	// shuffle stuff
	std::default_random_engine rand_engine( time( nullptr ) );
	shuffle_constants( chunk, rand_engine );
	shuffle_functions( chunk, rand_engine );

	// skip other debug info bc fuck it
	l_int count = read_int32( stream, little_endian );
	for ( l_int i = 0; i < count; ++i ) { // source line pos list
		read_int32( stream, little_endian );
	}

	count = read_int32( stream, little_endian );
	for ( l_int i = 0; i < count; ++i ) { // local list
		read_string( stream, little_endian );
		read_int32( stream, little_endian );
		read_int32( stream, little_endian );
	}

	count = read_int32( stream, little_endian );
	for ( l_int i = 0; i < count; ++i ) { // upvalue list
		read_string( stream, little_endian );
	}

	return chunk;
}


void shuffle_functions( chunk_t& chunk, std::default_random_engine rand_engine ) {
	l_int shuffle_map[ chunk.function_cnt ];
	for ( l_int i = 0; i < chunk.function_cnt; ++i )
		shuffle_map[i] = i;

	for ( l_int i = 0; i < chunk.function_cnt; ++i )
		std::swap( shuffle_map[ i ], shuffle_map[ rand_engine() % ( i + 1 ) ] );

	for ( l_int i = 0; i < chunk.instruction_cnt; ++i ) {
		instruction_t& instruction = chunk.instructions[i];

		// thanks lua for making so many bullshit instruction field rules!
		switch ( instruction.opcode ) {

			case OP_CLOSURE:
				instruction.b = shuffle_map[ instruction.b ];
				break;

			default: // shut up compiler
				break;

		}
	}

	for ( l_int i = 0; i < chunk.function_cnt; ++i ) {
		while ( shuffle_map[i] != i ) {
			std::swap( chunk.functions[ i ], chunk.functions[ shuffle_map[i] ] );
			std::swap( shuffle_map[ i ], shuffle_map[ shuffle_map[i] ] );
		}
	}
}
