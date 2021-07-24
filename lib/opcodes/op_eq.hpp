// TODO: create op_lt and op_le using this
#pragma once
#ifndef OP_EQ_HEADER_HPP
#define OP_EQ_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_eq_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_EQ;
	}

	std::string string() override {
		return R"(local KeyB, KeyC =
			Instr[2] > 255 and Constants[Instr[2] - 256] or Stack[Instr[2]],
			Instr[3] < 256 and Stack[Instr[3]] or Constants[Instr[3] - 256]
		if (Instr[1] == 0) == (KeyB == KeyC) then
			InstrPtr = InstrPtr + 1
		end)";
	}
};

class vop_eq_bc_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_EQ &&
			instruction.b <= 255 &&
			instruction.c <= 255;
	}

	std::string string() override {
		return "InstrPtr = InstrPtr + ((Stack[Instr[2]] ~= Stack[Instr[3]]) ~= not (Instr[1] ~= 0) and 1 or 0)";
	}
};

class vop_eq_bc_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_EQ &&
			instruction.b > 255 &&
			instruction.c <= 255;
	}

	std::string string() override {
		return R"(local Offset = 0
		if (Instr[1] ~= 0) == (Stack[Instr[3]] ~= Constants[Instr[2] - 256]) then
			Offset = Offset + 1
		end
		InstrPtr = InstrPtr + Offset)";
	}
};

class vop_eq_bc_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_EQ &&
			instruction.b <= 255 &&
			instruction.c > 255;
	}

	std::string string() override {
		return R"(local EQ = Stack[Instr[2]] == Constants[Instr[3] - 256]
		if EQ == not (Instr[1] ~= 0) then
			InstrPtr = InstrPtr + 1
		end)";
	}
};

class vop_eq_bc_4_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_EQ &&
			instruction.b > 255 &&
			instruction.c > 255;
	}

	std::string string() override {
		return R"(local EQ1 = Instr[1] == 0
		local EQ2 = Constants[Instr[3] - 256] ~= Constants[Instr[2] - 256]
		InstrPtr = EQ1 == EQ2 and InstrPtr or 1 + InstrPtr)";
	}
};

class vmut_eq_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine &rand_engine ) override {
		switch ( rand_engine() % 5 ) {
			case 0: return new vop_eq_1_t();
			case 1: return new vop_eq_bc_1_t();
			case 2: return new vop_eq_bc_2_t();
			case 3: return new vop_eq_bc_3_t();
			case 4: return new vop_eq_bc_4_t();
		}

		return nullptr;
	}
};

#endif
