#pragma once
#ifndef OP_SETGLOBAL_HEADER_HPP
#define OP_SETGLOBAL_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_setglobal_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SETGLOBAL;
	}

	std::string string() override {
		return "Env[Constants[Instr[2]]] = Stack[Instr[1]]";
	}
};

class vmut_setglobal_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		return new vop_setglobal_t();
	}
};


#endif
