#include <iostream>
#include <fstream>

#include "deserializer/deserializer.hpp"
#include "deserializer/datatypes.hpp"
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

	for ( l_int i = 0; i < chunk.instruction_cnt; ++i )
		instruction_update_refs( chunk, chunk.instructions[i] );

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
