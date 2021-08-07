#pragma once
#ifndef OP_NEWTABLE_HEADER_HPP
#define OP_NEWTABLE_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_newtable_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_NEWTABLE;
	}

	std::string string() override {
		return "Stack[Instr[1]] = {}";
	}
};

class vmut_newtable_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		return new vop_newtable_t();
	}
};

#endif
