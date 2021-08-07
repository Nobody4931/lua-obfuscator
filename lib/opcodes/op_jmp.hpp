#pragma once
#ifndef OP_JMP_HEADER_HPP
#define OP_JMP_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_jmp_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_JMP;
	}

	std::string string() override {
		return "InstrPtr = InstrPtr + Instr[2]";
	}
};

class vop_jmp_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_JMP;
	}

	std::string string() override {
		return "InstrPtr = Instr[2] + InstrPtr";
	}
};

class vmut_jmp_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine &rand_engine, bool base_case ) override {
		switch ( rand_engine() % 2 ) {
			case 0: return new vop_jmp_1_t();
			case 1: return new vop_jmp_2_t();
		}

		return nullptr;
	}
};

#endif
