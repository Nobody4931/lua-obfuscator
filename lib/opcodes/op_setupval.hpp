#pragma once
#ifndef OP_SETUPVAL_HEADER_HPP
#define OP_SETUPVAL_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_setupval_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SETUPVAL;
	}

	std::string string() override {
		return "Upvalues[Instr[2]] = Stack[Instr[1]]";
	}
};

class vmut_setupval_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine ) override {
		return new vop_setupval_t();
	}
};

#endif
