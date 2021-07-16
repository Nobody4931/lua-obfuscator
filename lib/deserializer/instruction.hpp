#pragma once
#ifndef INSTRUCTION_HEADER_HPP
#define INSTRUCTION_HEADER_HPP

#include <cstdint>

#include "deserializer/datatypes.hpp"

enum instr_t : uint8_t {
	i_ABC  = 0,
	i_ABx  = 1,
	i_AsBx = 2
};

enum opcode_t : uint8_t {
	OP_MOVE      = 0,
	OP_LOADK     = 1,
	OP_LOADBOOL  = 2,
	OP_LOADNIL   = 3,
	OP_GETUPVAL  = 4,
	OP_GETGLOBAL = 5,
	OP_GETTABLE  = 6,
	OP_SETGLOBAL = 7,
	OP_SETUPVAL  = 8,
	OP_SETTABLE  = 9,
	OP_NEWTABLE  = 10,
	OP_SELF      = 11,
	OP_ADD       = 12,
	OP_SUB       = 13,
	OP_MUL       = 14,
	OP_DIV       = 15,
	OP_MOD       = 16,
	OP_POW       = 17,
	OP_UNM       = 18,
	OP_NOT       = 19,
	OP_LEN       = 20,
	OP_CONCAT    = 21,
	OP_JMP       = 22,
	OP_EQ        = 23,
	OP_LT        = 24,
	OP_LE        = 25,
	OP_TEST      = 26,
	OP_TESTSET   = 27,
	OP_CALL      = 28,
	OP_TAILCALL  = 29,
	OP_RETURN    = 30,
	OP_FORLOOP   = 31,
	OP_FORPREP   = 32,
	OP_TFORLOOP  = 33,
	OP_SETLIST   = 34,
	OP_CLOSE     = 35,
	OP_CLOSURE   = 36,
	OP_VARARG    = 37
};

struct instruction_t {
	instr_t type;
	opcode_t opcode;

	l_instr data;
	int32_t a;
	int32_t b;
	int32_t c;
};

#endif
