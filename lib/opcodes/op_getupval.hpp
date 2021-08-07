#pragma once
#ifndef OP_GETUPVAL_HEADER_HPP
#define OP_GETUPVAL_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_getupval_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_GETUPVAL;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Upvalues[Instr[2]]";
	}
};

class vmut_getupval_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		return new vop_getupval_t();
	}
};

#endif
