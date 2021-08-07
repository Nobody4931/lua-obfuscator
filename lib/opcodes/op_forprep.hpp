#pragma once
#ifndef OP_FORPREP_HEADER_HPP
#define OP_FORPREP_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_forprep_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_FORPREP;
	}

	std::string string() override {
		return R"(local A = Instr[1]
		Stack[A] = Stack[A] - Stack[A + 2]
		InstrPtr = InstrPtr + Instr[2])";
	}
};

class vop_forprep_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_FORPREP;
	}

	std::string string() override {
		return R"(local SBX = Instr[2]
		InstrPtr = SBX + InstrPtr
		Stack[Instr[1]] = -Stack[2 - -Instr[1]] + Stack[Instr[1]])";
	}
};

class vmut_forprep_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		switch ( rand_engine() % 2 ) {
			case 0: return new vop_forprep_1_t();
			case 1: return new vop_forprep_2_t();
		}

		return nullptr;
	}
};

#endif
