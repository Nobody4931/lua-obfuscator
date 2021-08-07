#pragma once
#ifndef OP_UNM_HEADER_HPP
#define OP_UNM_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_unm_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_UNM;
	}

	std::string string() override {
		return "Stack[Instr[1]] = -Stack[Instr[2]]";
	}
};

class vmut_unm_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		return new vop_unm_t();
	}
};

#endif
