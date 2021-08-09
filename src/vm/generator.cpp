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
#include <cstring>
#include <iostream>
#include <sstream>
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

#define OBF_NAME "Nobody's Lua Obfuscator"
#define OBF_VERS "v1.0.0a"

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
	std::make_unique< vmut_vararg_t >(),    /* OP_VARARG */
};

static inline uint8_t unique_byte( std::set<uint8_t>& used, std::default_random_engine& rand_engine ) {
	uint8_t byte;
	do {
		byte = rand_engine() % 256;
	} while ( used.count( byte ) );
	used.insert( byte );
	return byte;
}

void generate_vm( chunk_t& chunk, std::stringstream& out ) {
	std::default_random_engine rand_engine( time( nullptr ) );

	// shuffle obfuscation order
	chunk_order_t chunk_order[5] {
		chunk_order_t::ORD_PARAM_CNT,
		chunk_order_t::ORD_UPVAL_CNT,
		chunk_order_t::ORD_INSTRUCTIONS,
		chunk_order_t::ORD_CONSTANTS,
		chunk_order_t::ORD_PROTOTYPES
	};

	instr_order_t instr_order[3] {
		instr_order_t::ORD_OPCODE,
		instr_order_t::ORD_ENUM,
		instr_order_t::ORD_FIELDS
	};

	std::shuffle( chunk_order, chunk_order + 5, rand_engine );
	std::shuffle( instr_order, instr_order + 3, rand_engine );

	// generate watermark xor key
	const char* watermark = "Obfuscated using " OBF_NAME " " OBF_VERS " / Contact: @Nobody#4931 via Discord";
	const size_t watermark_len = std::strlen( watermark );

	uint16_t watermark_key = 0;
	for ( size_t i = 0; i < watermark_len; ++i ) {
		watermark_key = ( watermark_key + watermark[ i ] * 227 + 83 ) % 479;
		for ( size_t j = 0; j + 1 < watermark_len; j += 3 ) {
			watermark_key ^= *reinterpret_cast<const uint16_t*>( watermark + j );
		}
	}

	// obfuscation context
	obfuscation_context_t context {
		.bytecode = std::vector<uint8_t>(),

		.chunk_order = chunk_order,
		.instr_order = instr_order,

		.const_map = std::map<const_t, uint8_t>(),
		.enum_map = std::map<instr_t, uint8_t>(),
		.opcode_map = std::map<opcode_t, std::vector<std::pair<uint8_t, vopcode_t*>>>(),

		.param_xor_key = static_cast<uint8_t>( rand_engine() % 256 ),
		.upval_xor_key = static_cast<uint8_t>( rand_engine() % 256 ),
		.chunk_xor_key = static_cast<uint8_t>( rand_engine() % 256 ),
		.instr_xor_key = static_cast<uint8_t>( rand_engine() % 256 ),
		.const_xor_key = static_cast<uint8_t>( rand_engine() % 256 ),
		.water_xor_key = static_cast<uint8_t>( watermark_key % 256 )
	};


	std::set<uint8_t> used_bytes;

	// generate constant mappings
	context.const_map[ const_t::K_NIL ] = unique_byte( used_bytes, rand_engine );
	context.const_map[ const_t::K_BOOLEAN ] = unique_byte( used_bytes, rand_engine );
	context.const_map[ const_t::K_NUMBER ] = unique_byte( used_bytes, rand_engine );
	context.const_map[ const_t::K_STRING ] = unique_byte( used_bytes, rand_engine );

	used_bytes.clear();

	// generate instruction type mappings
	context.enum_map[ instr_t::i_ABC ] = unique_byte( used_bytes, rand_engine );
	context.enum_map[ instr_t::i_ABx ] = unique_byte( used_bytes, rand_engine );
	context.enum_map[ instr_t::i_AsBx ] = unique_byte( used_bytes, rand_engine );

	used_bytes.clear();

	// generate opcode mutations
	for ( uint8_t i = 0; i < 38; ++i ) {
		// probably not the best way to do this but i changed my mind last minute so fuck off
		context.opcode_map[ static_cast<opcode_t>( i ) ].push_back( {
			unique_byte( used_bytes, rand_engine ),
			virtual_mutators[ i ]->mutate( rand_engine, true )
		} );

		uint8_t gen_cnt = 2 + rand_engine() % 3; // [2, 4]
		for ( uint8_t j = 0; j < gen_cnt; ++j ) {
			context.opcode_map[ static_cast<opcode_t>( i ) ].push_back( {
				unique_byte( used_bytes, rand_engine ),
				virtual_mutators[ i ]->mutate( rand_engine )
			} );
		}
	}


	serialize_chunk( context, chunk, true );

	// write variables
	out << "for _, Watermark in next, {\"" << watermark << "\"} do" R"(
	local String = string
	local Byte = String.byte
	local Char = String.char

	local Math = math
	local Abs = Math.abs
	local Max = Math.max
	local LDExp = Math.ldexp

	local Table = table
	local Insert = Table.insert

	local Debug = debug
	local GetInfo = Debug.getinfo

	local Select = select
	local Loadstring = loadstring
	local Setmetatable = setmetatable
	local GetFEnv = getfenv
	local Unpack = unpack

	local TMP1 = { 27, 76, 117, 97, 81, 0, 1, 4, 8, 4, 8, 0, 7, 0, 0, 0, 0, 0, 0, 0, 60, 101, 118, 97, 108, 62, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 1, 0, 0, 0, 30, 0, 0, 1, 30, 0, 128, 0, 1, 0, 0, 0, 4, 8, 0, 0, 0, 0, 0, 0, 0, 95, 95, 105, 110, 100, 101, 120, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
	local TMP2 = function()
		local Result = ""
		for Idx = 1, #TMP1 do
			Result = Result .. Char(TMP1[Idx])
		end
		return Result
	end

	local __Func = Loadstring("\27\76\117\97\81\0\1\4\8\4\8\0\7\0\0\0\0\0\0\0\60\101\118\97\108\62\0\1\0\0\0\3\0\0\0\0\0\0\2\3\0\0\0\1\0\0\0\30\0\0\1\30\0\128\0\1\0\0\0\4\5\0\0\0\0\0\0\0\102\117\110\99\0\0\0\0\0\3\0\0\0\2\0\0\0\2\0\0\0\3\0\0\0\0\0\0\0\0\0\0\0")()
	local __Index = Loadstring(TMP2())()

	TMP1[61] = 11
	for _, Idx in next, { 89, 28, 85, 93, 32 } do
		TMP1[Idx] = 1 + TMP1[Idx]
	end
	Insert(TMP1, 71, 119)
	Insert(TMP1, 71, 101)
	Insert(TMP1, 71, 110)

	local __NewIndex = Loadstring(TMP2())()

	local BitXOR = function(a,b)
		local p,c=1,0
		while a>0 and b>0 do
			local ra,rb=a%2,b%2
			if ra~=rb then c=c+p end
			a,b,p=(a-ra)/2,(b-rb)/2,p*2
		end
		if a<b then a=b end
		while a>0 do
			local ra=a%2
			if ra>0 then c=c+p end
			a,p=(a-ra)/2,p*2
		end
		return c
	end

	local BitOR = function(a,b)
		local p,c=1,0
		while a+b>0 do
			local ra,rb=a%2,b%2
			if ra+rb>0 then c=c+p end
			a,b,p=(a-ra)/2,(b-rb)/2,p*2
		end
		return c
	end

	local BitNOT = function(n)
		local p,c=1,0
		while n>0 do
			local r=n%2
			if r<1 then c=c+p end
			n,p=(n-r)/2,p*2
		end
		return c
	end

	local BitAND = function(a,b)
		local p,c=1,0
		while a>0 and b>0 do
			local ra,rb=a%2,b%2
			if ra+rb>1 then c=c+p end
			a,b,p=(a-ra)/2,(b-rb)/2,p*2
		end
		return c
	end
	)";


	// write deserialize function (assume little endian fuck it)
	// TODO: compressed bytecode bullshit
	out << R"(
	local TLChunk = (function(Bytecode)
		local WaterKey = 0
		for Idx = 1, #Watermark do
			WaterKey = ( WaterKey + Byte(Watermark, Idx) * 227 + 83 ) % 479
			for Idz = 1, #Watermark + -1, 3 do
				local XOR = 256 * Byte(Watermark, 1 + Idz) + Byte(Watermark, Idz)
				WaterKey = BitXOR(WaterKey, XOR)
			end
		end
		WaterKey = WaterKey % 256

		return (function(ReadPos)
			local ChunkKey = )" << static_cast<uint16_t>( context.chunk_xor_key ) << R"(

			local ReadByte = function(XORKey)
				local B = Byte(Bytecode, ReadPos, ReadPos)
				ReadPos = ReadPos + 1

				return BitXOR(B, XORKey)
			end

			local UpvalKey = )" << static_cast<uint16_t>( context.upval_xor_key ) << R"(

			local ReadInt16 = function(XORKey)
				local B1, B2 = Byte(Bytecode, ReadPos, ReadPos + 1)
				ReadPos = ReadPos + 2

				B1, B2 = BitXOR(B1, XORKey), BitXOR(B2, XORKey)
				return 256 * B2 + B1
			end

			local ConstKey = )" << static_cast<uint16_t>( context.param_xor_key ) << R"(

			local ReadInt32 = function(XORKey)
				local B1, B2, B3, B4 = Byte(Bytecode, ReadPos, ReadPos + 3)
				ReadPos = ReadPos + 4

				B1, B2, B3, B4 = BitXOR(B1, XORKey), BitXOR(B2, XORKey), BitXOR(B3, XORKey), BitXOR(B4, XORKey)
				return 16777216 * B4 + 65536 * B3 + 256 * B2 + B1
			end

			local InstrKey = )" << static_cast<uint16_t>( context.instr_xor_key ) << R"(

			local ReadDouble = function(XORKey)
				local Left = ReadInt32(XORKey)
				local Right = ReadInt32(XORKey)

				local Sign = (-1) ^ Max(1, BitAND(Right, 2147483648))
				local Exponent = BitAND(Right, 2146435072) / 1048576
				local Mantissa = BitAND(Right, 1048575) * 4294967296 + Left
				local IsNormal = 1

				if Exponent == 2047 then
					if Mantissa == 0 then
						return Sign * (1 / 0)
					else
						return Sign * (0 / 0)
					end
				elseif Exponent == 0 then
					if Mantissa == 0 then
						return Sign * 0
					end

					IsNormal = 0
					Exponent = 1
				end

				return LDExp(Sign, Exponent - 1023) * (IsNormal + Mantissa / (2 ^ 52))
			end

			local ParamKey = )" << static_cast<uint16_t>( context.param_xor_key ) << R"(

			local ReadString = function(XORKey)
				local Length = ReadInt32(XORKey)
				local Result = ''

				if Length == 0 then
					return Result
				end

				for Idx = 1, Length do
					Result = Result .. Char( ReadByte(XORKey) )
				end

				return Result
			end
	)";


	out << R"(
		end)(1)
	end))";

	// write bytecode
	out << "(\"";
	for ( uint8_t byte : context.bytecode ) {
		out << '\\' << static_cast<uint16_t>( byte );
	}
	out << "\")\n";

	// finish obfuscation
	out << "return Wrap({}, GetFEnv(0), TLChunk)()\n";
	out << "end";
}
