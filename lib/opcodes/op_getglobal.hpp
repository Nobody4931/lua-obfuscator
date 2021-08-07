#pragma once
#ifndef OP_GETGLOBAL_HEADER_HPP
#define OP_GETGLOBAL_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_getglobal_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_GETGLOBAL;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Env[Constants[Instr[2]]]";
	}
};

class vmut_getglobal_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		return new vop_getglobal_t();
	}
};

#endif
