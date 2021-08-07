#pragma once
#ifndef OP_GETTABLE_HEADER_HPP
#define OP_GETTABLE_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_gettable_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_GETTABLE;
	}

	std::string string() override {
		return R"(local Key
		if Instr[3] > 255 then
			Key = Constants[Instr[3] - 256]
		else
			Key = Stack[Instr[3]]
		end
		Stack[Instr[1]] = Stack[Instr[2]][Key])";
	}
};

class vop_gettable_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_GETTABLE;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Stack[Instr[2]][ Instr[3] <= 255 and Stack[Instr[3]] or Constants[Instr[3] - 256] ]";
	}
};

class vop_gettable_c1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_GETTABLE &&
			instruction.c > 255;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Stack[Instr[2]][Constants[Instr[3] - 256]]";
	}
};

class vop_gettable_c2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_GETTABLE &&
			instruction.c <= 255;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Stack[Instr[2]][Stack[Instr[3]]]";
	}
};

class vmut_gettable_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		switch ( rand_engine() % ( base_case ? 2 : 4 ) ) {
			case 0: return new vop_gettable_1_t();
			case 1: return new vop_gettable_2_t();
			case 2: return new vop_gettable_c1_t();
			case 3: return new vop_gettable_c2_t();
		}

		return nullptr;
	}
};

#endif
