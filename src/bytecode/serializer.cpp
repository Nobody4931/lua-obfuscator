#include <cstdint>
#include <random>
#include <ctime>

#include "bytecode/serializer.hpp"
#include "bytecode/datatypes.hpp"
#include "bytecode/instruction.hpp"
#include "bytecode/constant.hpp"
#include "bytecode/chunk.hpp"
#include "vm/generator.hpp"

static inline void serialize_instruction( obfuscation_context_t& context, instruction_t& instruction, bool little_endian ) {
	static std::default_random_engine rand_engine( time( nullptr ) );

	for ( uint8_t i = 0; i < 3; ++i ) {
		instr_t type = instruction_mappings[ instruction.opcode ];

		uint8_t op_idx = rand_engine() % context.opcode_map[ instruction.opcode ].size();
		while ( !context.opcode_map[ instruction.opcode ][ op_idx ].second->valid( instruction ) )
			op_idx = ( op_idx + 1 ) % context.opcode_map[ instruction.opcode ].size();

		switch ( context.instr_order[i] ) {

			case instr_order_t::ORD_OPCODE:
				write_byte( context.bytecode, context.opcode_map[ instruction.opcode ][ op_idx ].first );
				break;

			case instr_order_t::ORD_ENUM:
				write_byte( context.bytecode, type );
				break;

			case instr_order_t::ORD_FIELDS: // always 5 bytes
				switch ( type ) {

					case instr_t::i_ABC:
						write_int16( context.bytecode, instruction.b, little_endian );
						write_int16( context.bytecode, instruction.c, little_endian );
						break;

					case instr_t::i_ABx:
						write_int32( context.bytecode, instruction.b, little_endian );
						break;

					case instr_t::i_AsBx:
						write_int32( context.bytecode, instruction.b + ( (1 << 17) - 1 ), little_endian );
						break;

				}

				write_byte( context.bytecode, instruction.a );
				break;

		}
	}
}

static inline void serialize_constant( obfuscation_context_t& context, constant_t& constant, bool little_endian ) {
	switch ( constant.type ) {

		case const_t::K_NIL: // shut up compiler
			break;

		case const_t::K_BOOLEAN:
			write_byte( context.bytecode, *constant.data );
			break;

		case const_t::K_NUMBER:
			write_double( context.bytecode, *reinterpret_cast<l_number*>( constant.data ), little_endian );
			break;

		case const_t::K_STRING:
			write_some( context.bytecode, constant.data, reinterpret_cast<l_string*>( constant.data )->size, true, little_endian );
			break;

	}
}

// TODO: serialize upvalue count bc op_closure uses it lol
void serialize_chunk( obfuscation_context_t& context, chunk_t& chunk, bool little_endian ) {
	for ( uint8_t i = 0; i < 4; ++i ) {
		switch ( context.chunk_order[i] ) {

			case chunk_order_t::ORD_PARAM_CNT:
				write_byte( context.bytecode, chunk.parameter_cnt );
				break;

			case chunk_order_t::ORD_INSTRUCTIONS:
				write_int32( context.bytecode, chunk.instruction_cnt, little_endian );
				for ( l_int i = 0; i < chunk.instruction_cnt; ++i )
					serialize_instruction( context, chunk.instructions[i], little_endian );
				break;

			case chunk_order_t::ORD_CONSTANTS: // security probably wtf
				write_int32( context.bytecode, chunk.constant_cnt, little_endian );
				for ( l_int i = 0; i < chunk.constant_cnt; ++i )
					write_byte( context.bytecode, context.const_map[ chunk.constants[i].type ] );
				for ( l_int i = 0; i < chunk.constant_cnt; ++i )
					serialize_constant( context, chunk.constants[i], little_endian );
				break;

			case chunk_order_t::ORD_PROTOTYPES:
				write_int32( context.bytecode, chunk.function_cnt, little_endian );
				for ( l_int i = 0; i < chunk.function_cnt; ++i )
					serialize_chunk( context, chunk.functions[i], little_endian );
				break;

		}
	}
}
