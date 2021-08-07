#pragma once
#ifndef OP_NOT_HEADER_HPP
#define OP_NOT_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_not_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_NOT;
	}

	std::string string() override {
		return "Stack[Instr[1]] = not Stack[Instr[2]]";
	}
};

class vmut_not_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		return new vop_not_t();
	}
};

#endif
