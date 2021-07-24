#pragma once
#ifndef OP_CONCAT_HEADER_HPP
#define OP_CONCAT_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_concat_t : public vopcode_t {
public:
	bool valid( instruction_t &instruction ) override {
		return instruction.opcode == OP_CONCAT;
	}

	std::string string() override {
		return R"(local Offset = Instr[2] + 1
		local Result = Stack[Offset - 1]
		for Idx = Offset, Instr[3] do
			Result = Result .. Stack[Idx]
		end
		Stack[Instr[1]] = Result)";
	}
};

class vmut_concat_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine &rand_engine ) override {
		return new vop_concat_t();
	}
};

#endif
