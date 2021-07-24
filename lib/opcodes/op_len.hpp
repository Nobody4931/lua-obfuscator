#pragma once
#ifndef OP_LEN_HEADER_HPP
#define OP_LEN_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_len_t : public vopcode_t {
	bool valid( instruction_t &instruction ) override {
		return instruction.opcode == OP_LEN;
	}

	std::string string() override {
		return "Stack[Instr[1]] = #Stack[Instr[2]]";
	}
};

class vmut_len_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine ) override {
		return new vop_len_t();
	}
};

#endif
