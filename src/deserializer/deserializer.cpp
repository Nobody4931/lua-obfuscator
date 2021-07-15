#include <iostream>
#include <fstream>

#include "deserializer/deserializer.hpp"
#include "deserializer/datatypes.hpp"
#include "deserializer/chunk.hpp"

chunk_t decode_file( std::ifstream& stream ) {
	uint8_t* signature = read_some( stream, 4, false );
	if ( signature[0] != 0x1B || signature[1] != 0x4C || signature[2] != 0x75 || signature[3] != 0x61 ) {
		std::cerr << "Input file does not contain Lua bytecode" << std::endl;
		throw "INVALID_FILE";
	}

	delete[] signature;

	if ( read_byte( stream ) != 0x51 ) {
		std::cerr << "Input file was not precompiled using Lua 5.1" << std::endl;
		throw "UNSUPPORTED_VERSION";
	}

	read_byte( stream ); // version official wtf

	bool little_endian = read_byte( stream ) == 1;

	// fuck it not supporting anything other than defaults
	if ( read_byte( stream ) != 4 ||    // int size
			read_byte( stream ) != 4 || // size_t size
			read_byte( stream ) != 4 || // instruction size
			read_byte( stream ) != 8 || // number size
			read_byte( stream ) != 0 ) {  // number integer bullshit

		std::cerr << "Input file uses unsupported options" << std::endl;
		throw "UNSUPPORTED_OPTION";
	}

	return decode_chunk( stream, little_endian );
}
