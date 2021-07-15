#include <iostream>
#include <fstream>

#include "deserializer/deserializer.hpp"
#include "deserializer/datatypes.hpp"
#include "deserializer/constant.hpp"
#include "deserializer/chunk.hpp"

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
