#pragma once
#ifndef GENERATOR_HEADER_HPP
#define GENERATOR_HEADER_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <map>

#include "bytecode/instruction.hpp"
#include "bytecode/chunk.hpp"

enum class chunk_order_t : uint8_t {
	ORD_PARAM_CNT    = 0, // i think this is needed?
	ORD_INSTRUCTIONS = 1,
	ORD_CONSTANTS    = 2,
	ORD_PROTOTYPES   = 3
};

enum class instr_order_t : uint8_t {
	ORD_OPCODE = 0,
	ORD_ENUM   = 1,
	ORD_FIELDS = 2
};

struct obfuscation_context_t {
	std::vector<uint8_t> bytecode;

	chunk_order_t* chunk_order;
	instr_order_t* instr_order;

	std::map<const_t, uint8_t> const_map;
	std::map<opcode_t, std::vector<uint8_t>> opcode_map;

	uint8_t param_xor_key;
	uint8_t chunk_xor_key;
	uint8_t instr_xor_key;
	uint8_t const_xor_key;
};

void generate_vm( chunk_t& chunk, std::string& out );

#endif
