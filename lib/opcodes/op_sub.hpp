#pragma once
#ifndef OP_SUB_HEADER_HPP
#define OP_SUB_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_sub_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SUB;
	}

	std::string string() override {
		return R"(local KeyB, KeyC = function(Self, Field)
			return Self, Instr[Field] <= 255 and Stack[Instr[Field]] or Constants[Instr[Field] - 256]
		end
		KeyB, KeyC = KeyB(KeyB, 3)
		Stack[Instr[1]] = KeyB(3, 2) * -1 + KeyC
		Stack[Instr[1]] = -Stack[Instr[1]])";
	}
};

class vop_sub_bc_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SUB &&
			instruction.b <= 255 &&
			instruction.c <= 255;
	}

	std::string string() override {
		return "Stack[Instr[1]] = -Stack[Instr[3]] + Stack[Instr[2]]";
	}
};

class vop_sub_bc_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SUB &&
			instruction.b > 255 &&
			instruction.c <= 255;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Constants[Instr[2] - 256] - Stack[Instr[3]]";
	}
};

class vop_sub_bc_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SUB &&
			instruction.b <= 255 &&
			instruction.c > 255;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Stack[Instr[2]] + -Constants[Instr[3] - 256]";
	}
};

class vop_sub_bc_4_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SUB &&
			instruction.b > 255 &&
			instruction.c > 255;
	}

	std::string string() override {
		return "Stack[Instr[1]] = -Constants[Instr[3] - 256] - -Constants[Instr[2] - 256]";
	}
};

class vmut_sub_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine ) override {
		switch ( rand_engine() % 5 ) {
			case 0: return new vop_sub_t();
			case 1: return new vop_sub_bc_1_t();
			case 2: return new vop_sub_bc_2_t();
			case 3: return new vop_sub_bc_3_t();
			case 4: return new vop_sub_bc_4_t();
		}

		return nullptr;
	}
};

#endif
