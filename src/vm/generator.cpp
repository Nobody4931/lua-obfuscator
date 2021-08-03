// OBFUSCATOR VARIABLES

// IsMoveOp() : Checks if an instruction corresponds to a MOVE operation
// IsGetupvalOp() : Checks if an instruction corresponds to a GETUPVAL operation

// Upvalues[] : The upvalues
// Env{} : The environment
// Stack[] : The stack
// Constants[] : The constant list
// Instrs[] : The instruction list
//	- Instr[] : An individual instruction
//		- 1 : Field A
//		- 2 : Field B
//		- 3 : Field C
//		- 4 : Opcode enum
//		- 5 : Entire data of instruction
// Protos[] : The function prototypes list
//	- Proto[] : A binary chunk
//		- 1 : Parameter count
//		- 2 : Upvalue count
//		- 3 : Instructions
//		- 4 : Constants
//		- 5 : Prototypes
// LUpvalues[] : The list of a list of locals used as upvalues
//	- Locals[] : The list of locals used as upvalues
//		- LUpvalue[] : An individual local upvalue
//			- 1 : The memory where this local upvalue is stored
//			- 2 : The register index in which this local upvalue is stored

#include <algorithm>
#include <utility>
#include <memory>
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
#include "opcodes/vopcode.hpp"
#include "opcodes/opcodes.hpp"
#include "vm/generator.hpp"

// TODO: create rest of the opcodes

// because for some fucking reason i trusted the words of some random guy on discord
static std::unique_ptr<vmutator_t> virtual_mutators[] {
	std::make_unique< vmut_move_t >(),      /* OP_MOVE */
	std::make_unique< vmut_loadk_t >(),     /* OP_LOADK */
	std::make_unique< vmut_loadbool_t >(),  /* OP_LOADBOOL */
	std::make_unique< vmut_loadnil_t >(),   /* OP_LOADNIL */
	std::make_unique< vmut_getupval_t >(),  /* OP_GETUPVAL */
	std::make_unique< vmut_getglobal_t >(), /* OP_GETGLOBAL */
	std::make_unique< vmut_gettable_t >(),  /* OP_GETTABLE */
	std::make_unique< vmut_setglobal_t >(), /* OP_SETGLOBAL */
	std::make_unique< vmut_setupval_t >(),  /* OP_SETUPVAL */
	std::make_unique< vmut_settable_t >(),  /* OP_SETTABLE */
	std::make_unique< vmut_newtable_t >(),  /* OP_NEWTABLE */
	std::make_unique< vmut_self_t >(),      /* OP_SELF */
	std::make_unique< vmut_add_t >(),       /* OP_ADD */
	std::make_unique< vmut_sub_t >(),       /* OP_SUB */
	std::make_unique< vmut_mul_t >(),       /* OP_MUL */
	std::make_unique< vmut_div_t >(),       /* OP_DIV */
	std::make_unique< vmut_mod_t >(),       /* OP_MOD */
	std::make_unique< vmut_pow_t >(),       /* OP_POW */
	std::make_unique< vmut_unm_t >(),       /* OP_UNM */
	std::make_unique< vmut_not_t >(),       /* OP_NOT */
	std::make_unique< vmut_len_t >(),       /* OP_LEN */
	std::make_unique< vmut_concat_t >(),    /* OP_CONCAT */
	std::make_unique< vmut_jmp_t >(),       /* OP_JMP */
	std::make_unique< vmut_eq_t >(),        /* OP_EQ */
	std::make_unique< vmut_lt_t >(),        /* OP_LT */
	std::make_unique< vmut_le_t >(),        /* OP_LE */
	std::make_unique< vmut_test_t >(),      /* OP_TEST */
	std::make_unique< vmut_testset_t >(),   /* OP_TESTSET */
	std::make_unique< vmut_call_t >(),      /* OP_CALL */
	std::make_unique< vmut_tailcall_t >(),  /* OP_TAILCALL */
	std::make_unique< vmut_return_t >(),    /* OP_RETURN */
	std::make_unique< vmut_forloop_t >(),   /* OP_FORLOOP */
	std::make_unique< vmut_forprep_t >(),   /* OP_FORPREP */
	std::make_unique< vmut_tforloop_t >(),  /* OP_TFORLOOP */
	std::make_unique< vmut_setlist_t >(),   /* OP_SETLIST */
	std::make_unique< vmut_close_t >(),     /* OP_CLOSE */
	std::make_unique< vmut_closure_t >(),   /* OP_CLOSURE */
};

static inline uint8_t unique_byte( std::set<uint8_t>& used, std::default_random_engine& rand_engine ) {
	uint8_t byte;
	do {
		byte = rand_engine() % 256;
	} while ( used.count( byte ) );
	used.insert( byte );
	return byte;
}

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
		.opcode_map = std::map<opcode_t, std::vector<std::pair<uint8_t, vopcode_t*>>>(),

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
			// TODO: make sure at least one base case mutation is used each time
			// else some opcodes wont fuckin work
			context.opcode_map[ static_cast<opcode_t>( i ) ].push_back( {
				unique_byte( used_bytes, rand_engine ),
				virtual_mutators[ i ]->mutate( rand_engine )
			} );
		}
	}

	serialize_chunk( context, chunk );

	// TODO: remove testing code later
	for ( uint8_t byte : context.bytecode )
		std::cout << static_cast<uint16_t>( byte ) << ' ';
	std::cout << '\n';
}
