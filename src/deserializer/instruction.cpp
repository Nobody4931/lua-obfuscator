#include <iostream>
#include <fstream>

#include "deserializer/deserializer.hpp"
#include "deserializer/datatypes.hpp"
#include "deserializer/instruction.hpp"
#include "deserializer/chunk.hpp"

static const instr_t instruction_mappings[] {
	i_ABC,		/* OP_MOVE */
	i_ABx,		/* OP_LOADK */
	i_ABC,		/* OP_LOADBOOL */
	i_ABC,		/* OP_LOADNIL */
	i_ABC,		/* OP_GETUPVAL */
	i_ABx,		/* OP_GETGLOBAL */
	i_ABC,		/* OP_GETTABLE */
	i_ABx,		/* OP_SETGLOBAL */
	i_ABC,		/* OP_SETUPVAL */
	i_ABC,		/* OP_SETTABLE */
	i_ABC,		/* OP_NEWTABLE */
	i_ABC,		/* OP_SELF */
	i_ABC,		/* OP_ADD */
	i_ABC,		/* OP_SUB */
	i_ABC,		/* OP_MUL */
	i_ABC,		/* OP_DIV */
	i_ABC,		/* OP_MOD */
	i_ABC,		/* OP_POW */
	i_ABC,		/* OP_UNM */
	i_ABC,		/* OP_NOT */
	i_ABC,		/* OP_LEN */
	i_ABC,		/* OP_CONCAT */
	i_AsBx,		/* OP_JMP */
	i_ABC,		/* OP_EQ */
	i_ABC,		/* OP_LT */
	i_ABC,		/* OP_LE */
	i_ABC,		/* OP_TEST */
	i_ABC,		/* OP_TESTSET */
	i_ABC,		/* OP_CALL */
	i_ABC,		/* OP_TAILCALL */
	i_ABC,		/* OP_RETURN */
	i_AsBx,		/* OP_FORLOOP */
	i_AsBx,		/* OP_FORPREP */
	i_ABC,		/* OP_TFORLOOP */
	i_ABC,		/* OP_SETLIST */
	i_ABC,		/* OP_CLOSE */
	i_ABx,		/* OP_CLOSURE */
	i_ABC,		/* OP_VARARG */
};

void decode_instructions( chunk_t& chunk, std::ifstream& stream, bool little_endian ) {
	chunk.instruction_cnt = read_int32( stream, little_endian );
	chunk.instructions = new instruction_t[ chunk.instruction_cnt ];

	for ( l_int i = 0; i < chunk.instruction_cnt; ++i ) {
		l_instr data = read_int32( stream, little_endian );
		opcode_t opcode = static_cast<opcode_t>( data & 0b111111 );
		instr_t type = instruction_mappings[ opcode ];

		chunk.instructions[i] = {
			.type = type,
			.opcode = opcode,

			.data = data,
			.a = static_cast<int32_t>( (data >> 6) & ((1 << 8) - 1) )
		};

		// WHAT THE FUCK LUA
		switch ( type ) {

			case instr_t::i_ABC:
				chunk.instructions[i].c = static_cast<int32_t>( (data >> (6 + 8)) & ((1 << 9) - 1) );
				chunk.instructions[i].b = static_cast<int32_t>( (data >> (6 + 8 + 9)) & ((1 << 9) - 1) );
				break;

			case instr_t::i_ABx:
				chunk.instructions[i].b = static_cast<int32_t>( (data >> (6 + 8)) & ((1 << 18) - 1) );
				break;

			case instr_t::i_AsBx:
				chunk.instructions[i].b = static_cast<int32_t>( ( (data >> (6 + 8)) & ((1 << 18) - 1) ) - ((1 << 17) - 1) );
				break;

		}
	}
}
