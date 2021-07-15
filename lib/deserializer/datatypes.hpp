#pragma once
#ifndef DATATYPES_HEADER_HPP
#define DATATYPES_HEADER_HPP

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <fstream>

typedef uint32_t l_int;
typedef uint32_t l_size;
typedef uint32_t l_instr;
typedef double l_number;

struct l_string {
	l_size size;
	uint8_t* data;
};


inline bool is_little_endian() {
	union {
		uint32_t int32;
		uint8_t int8[4];
	} aabbccdd = { 0xAABBCCDD };

	return aabbccdd.int8[0] == 0xDD;
}

inline uint8_t* read_some( std::ifstream& stream, size_t count, bool factor_endianness, bool little_endian = true ) {
	uint8_t* bytes = new uint8_t[ count ];

	stream.read( reinterpret_cast<char*>( bytes ), count );
	if ( factor_endianness && little_endian != is_little_endian() )
		std::reverse( bytes, bytes + count );

	return bytes;
}

inline uint8_t read_byte( std::ifstream& stream ) {
	uint8_t bytes[1];
	stream.read( reinterpret_cast<char*>( bytes ), 1 );
	return bytes[0];
}

// not writing a separate function for reading l_int and l_size fuck off
inline uint32_t read_int32( std::ifstream& stream, bool little_endian ) {
	uint8_t* bytes = read_some( stream, 4, true, little_endian );
	uint32_t result = *reinterpret_cast<uint32_t*>( bytes );

	delete[] bytes;
	return result;
}

inline uint64_t read_int64( std::ifstream& stream, bool little_endian ) {
	uint8_t* bytes = read_some( stream, 8, true, little_endian );
	uint64_t result = *reinterpret_cast<uint64_t*>( bytes );

	delete[] bytes;
	return result;
}

inline double read_double( std::ifstream& stream, bool little_endian ) {
	uint8_t* bytes = read_some( stream, 8, true, little_endian );
	double result = *reinterpret_cast<double*>( bytes );

	delete[] bytes;
	return result;
}

inline l_string read_string( std::ifstream& stream, bool little_endian ) {
	l_size size = read_int32( stream, little_endian );
	if ( size == 0 )
		return { size, nullptr };
	return { size, read_some( stream, size, true, little_endian ) };
}

#endif
