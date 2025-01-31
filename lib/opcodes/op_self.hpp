#pragma once
#ifndef OP_SELF_HEADER_HPP
#define OP_SELF_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_self_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SELF;
	}

	std::string string() override {
		return R"(Stack[Instr[1]] = Stack[Instr[2]][ Instr[3] >= 256 and Constants[Instr[3] - 256] or Stack[Instr[3]] ]
		Stack[Instr[1] + 1] = Stack[Instr[2]])";
	}
};

class vop_self_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SELF;
	}

	std::string string() override {
		return R"(local A = Instr[1]
		Stack[A + 1] = Stack[Instr[2]]
		A = A + 1
		Stack[A - 1] = Stack[A][ Instr[3] > 255 and Constants[Instr[3] - 256] or Stack[Instr[3]] ])";
	}
};

class vop_self_c1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SELF &&
			instruction.c <= 255;
	}

	std::string string() override {
		return R"(Stack[Instr[1] + 1] = Stack[Instr[2]]
		Stack[Instr[1]] = Stack[Instr[2]][ Stack[Instr[3]] ])";
	}
};

class vop_self_c2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SELF &&
			instruction.c > 255;
	}

	std::string string() override {
		return R"(local Table = Stack[Instr[2]]
		Stack[Instr[1]] = Table[ Constants[Instr[3] - 256] ]
		Instr[1] = Instr[1] + 1
		Stack[Instr[1]] = Table
		Instr[1] = Instr[1] - 1)";
	}
};

class vmut_self_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		switch ( rand_engine() % ( base_case ? 2 : 4 ) ) {
			case 0: return new vop_self_1_t();
			case 1: return new vop_self_2_t();
			case 2: return new vop_self_c1_t();
			case 3: return new vop_self_c2_t();
		}

		return nullptr;
	}
};

#endif
