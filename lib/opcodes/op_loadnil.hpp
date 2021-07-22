#pragma once
#ifndef OP_LOADNIL_HEADER_HPP
#define OP_LOADNIL_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_loadnil_1_t : public vopcode_t {
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADNIL;
	}

	std::string string() override {
		return R"(for Idx = Instr[1], Instr[2] do
			Stack[Idx] = nil
		end)";
	}
};

class vop_loadnil_2_t : public vopcode_t {
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADNIL;
	}

	std::string string() override {
		return R"(local Idx = Instr[1]
		while Idx <= Instr[2] do
			Stack[Idx] = nil
			Idx = Idx + 1
		end)";
	}
};

class vop_loadnil_3_t : public vopcode_t {
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADNIL;
	}

	std::string string() override {
		return R"(local Offset = 0
		repeat
			Stack[Instr[1] + Offset] = nil
			Offset = Offset + 1
		until Offset + Instr[1] > Instr[2])";
	}
};

class vop_loadnil_ab_1_t : public vopcode_t {
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADNIL && instruction.a == instruction.b;
	}

	std::string string() override {
		return "Stack[Instr[1]] = nil";
	}
};

class vop_loadnil_ab_2_t : public vopcode_t {
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADNIL && instruction.a == instruction.b;
	}

	std::string string() override {
		return "Stack[Instr[2]] = nil";
	}
};

class vmut_loadnil_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine ) override {
		switch ( rand_engine() % 5 ) {
			case 0: return new vop_loadnil_1_t();
			case 1: return new vop_loadnil_2_t();
			case 2: return new vop_loadnil_3_t();
			case 3: return new vop_loadnil_ab_1_t();
			case 4: return new vop_loadnil_ab_2_t();
		}

		return nullptr;
	}
};

#endif
