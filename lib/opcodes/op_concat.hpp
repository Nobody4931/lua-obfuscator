#pragma once
#ifndef OP_CONCAT_HEADER_HPP
#define OP_CONCAT_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_concat_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CONCAT;
	}

	std::string string() override {
		return R"(local Result = Stack[Instr[2]]
		for Idx = Instr[2] + 1, Instr[3] do
			Result = Result .. Stack[Idx]
		end
		Stack[Instr[1]] = Result)";
	}
};

class vop_concat_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CONCAT;
	}

	std::string string() override {
		return R"(local Offset = Instr[2] + 1
		local Result = Stack[Offset - 1]
		while Offset <= Instr[3] do
			Result = Result .. Stack[Offset]
			Offset = Offset + 1
		end
		Stack[Instr[1]] = Result)";
	}
};

class vop_concat_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CONCAT;
	}

	std::string string() override {
		return R"(Stack[Instr[1]] = Stack[Instr[2]]
		local Offset = Instr[2]
		repeat
			Offset = Offset + 1
			Stack[Instr[1]] = Stack[Instr[1]] .. Stack[Offset]
		until Offset == Instr[3])";
	}
};

class vop_concat_bc_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CONCAT &&
			instruction.b + 1 == instruction.c;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Stack[Instr[2]] .. Stack[Instr[3]]";
	}
};

class vop_concat_bc_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CONCAT &&
			instruction.b + 1 == instruction.c;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Stack[Instr[2]] .. Stack[Instr[2] + 1]";
	}
};

class vop_concat_bc_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CONCAT &&
			instruction.b + 1 == instruction.c;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Stack[Instr[3] - 1] .. Stack[Instr[3]]";
	}
};

class vmut_concat_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine &rand_engine, bool base_case ) override {
		switch ( rand_engine() % ( base_case ? 3 : 6 ) ) {
			case 0: return new vop_concat_1_t();
			case 1: return new vop_concat_2_t();
			case 2: return new vop_concat_3_t();
			case 3: return new vop_concat_bc_1_t();
			case 4: return new vop_concat_bc_2_t();
			case 5: return new vop_concat_bc_3_t();
		}

		return nullptr;
	}
};

#endif
