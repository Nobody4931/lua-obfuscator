#pragma once
#ifndef OP_MOVE_HEADER_HPP
#define OP_MOVE_HEADER_HPP

#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_move_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_MOVE;
	}

	std::string string() override {
		return "Stack[OP_A] = Stack[OP_B]";
	}
};

class vmut_move_t : public vmutator_t {
public:
	vopcode_t* mutate() override {
		return new vop_move_t();
	};
};

#endif
