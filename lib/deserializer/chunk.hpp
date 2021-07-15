#pragma once
#ifndef CHUNK_HEADER_HPP
#define CHUNK_HEADER_HPP

#include <cstdint>
#include <iostream>
#include <fstream>

#include "deserializer/datatypes.hpp"
#include "deserializer/instruction.hpp"
#include "deserializer/constant.hpp"

enum vararg_flag_t : uint8_t {
	VA_HASARG   = (1 << 0),
	VA_ISVARARG = (1 << 1),
	VA_NEEDSARG = (1 << 2),
};

struct chunk_t {
	l_string name;
	l_int line;
	l_int line_l;

	uint8_t upvalue_cnt;
	uint8_t parameter_cnt;
	uint8_t vararg_flag;
	uint8_t stack_max;

	l_int instruction_cnt;
	instruction_t* instructions;

	l_int constant_cnt;
	constant_t* constants;

	l_int function_cnt;
	chunk_t* functions;
};

#endif
