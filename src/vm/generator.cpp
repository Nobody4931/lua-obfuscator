#include <random>
#include <ctime>
#include <string>

#include "bytecode/chunk.hpp"
#include "vm/generator.hpp"

void generate_vm( chunk_t& chunk, std::string& out ) {
	std::default_random_engine rand_engine( time( nullptr ) );

	chunk_order_t chunk_order[4] {
		chunk_order_t::ORD_PARAM_CNT,
		chunk_order_t::ORD_INSTRUCTIONS,
		chunk_order_t::ORD_CONSTANTS,
		chunk_order_t::ORD_PROTOTYPES
	};

	instr_order_t instr_order[3] {
		instr_order_t::ORD_OPCODE,
		instr_order_t::ORD_ENUM,
		instr_order_t::ORD_FIELDS
	};

	obfuscation_context_t context {
		.chunk_order = chunk_order,
		.instr_order = instr_order,
		.param_xor_key = static_cast<uint8_t>( rand_engine() % 256 ),
		.chunk_xor_key = static_cast<uint8_t>( rand_engine() % 256 ),
		.instr_xor_key = static_cast<uint8_t>( rand_engine() % 256 ),
		.const_xor_key = static_cast<uint8_t>( rand_engine() % 256 )
	};
}
