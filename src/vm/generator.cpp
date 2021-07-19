#include <algorithm>
#include <random>
#include <ctime>
#include <string>
#include <vector>
#include <set>
#include <map>

#include "bytecode/chunk.hpp"
#include "bytecode/constant.hpp"
#include "bytecode/instruction.hpp"
#include "bytecode/serializer.hpp"
#include "vm/generator.hpp"

static inline uint8_t unique_byte( std::set<uint8_t>& used, std::default_random_engine& rand_engine ) {
	uint8_t byte;
	do {
		byte = rand_engine() % 256;
	} while ( used.count( byte ) );
	used.insert( byte );
	return byte;
}

// TODO: create map for opcode_t -> vopcode_t* instances (ptr bc polymorphism or some shit)

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

	std::shuffle( chunk_order, chunk_order + 4, rand_engine );
	std::shuffle( instr_order, instr_order + 3, rand_engine );

	obfuscation_context_t context {
		.bytecode = std::vector<uint8_t>(),

		.chunk_order = chunk_order,
		.instr_order = instr_order,

		.const_map = std::map<const_t, uint8_t>(),
		.opcode_map = std::map<opcode_t, std::vector<uint8_t>>(),

		.param_xor_key = static_cast<uint8_t>( rand_engine() % 256 ),
		.chunk_xor_key = static_cast<uint8_t>( rand_engine() % 256 ),
		.instr_xor_key = static_cast<uint8_t>( rand_engine() % 256 ),
		.const_xor_key = static_cast<uint8_t>( rand_engine() % 256 )
	};

	std::set<uint8_t> used_bytes;

	context.const_map[ const_t::K_NIL ] = unique_byte( used_bytes, rand_engine );
	context.const_map[ const_t::K_BOOLEAN ] = unique_byte( used_bytes, rand_engine );
	context.const_map[ const_t::K_NUMBER ] = unique_byte( used_bytes, rand_engine );
	context.const_map[ const_t::K_STRING ] = unique_byte( used_bytes, rand_engine );

	used_bytes.clear();

	for ( uint8_t i = 0; i < 38; ++i ) {
		uint8_t gen_cnt = 3 + rand_engine() % 3; // [3, 5]
		for ( uint8_t j = 0; j < gen_cnt; ++j ) {
			context.opcode_map[ static_cast<opcode_t>( i ) ]
				.push_back( unique_byte( used_bytes, rand_engine ) );
		}
	}

	serialize_chunk( context, chunk );
}
