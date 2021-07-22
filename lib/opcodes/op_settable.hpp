#pragma once
#ifndef OP_SETTABLE_HEADER_HPP
#define OP_SETTABLE_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_settable_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SETTABLE;
	}

	std::string string() override {
		return R"(local KeyC, KeyB = function(Field, Self)
			if Field < 256 then
				return Stack[Field], Self
			end
			return Constants[Field - 256], Self
		end
		KeyC, KeyB = KeyC(Instr[3], KeyC)
		Stack[Instr[1]][ KeyB(Instr[2], 1) ] = KeyC)";
	}
};

class vop_settable_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SETTABLE;
	}

	std::string string() override {
		return "Stack[Instr[1]][ Instr[2] >= 256 and Constants[Instr[2] - 256] or Stack[Instr[2]] ] = Instr[3] <= 255 and Stack[Instr[3]] or Constants[Instr[3] - 256]";
	}
};

class vop_settable_bc_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SETTABLE &&
			instruction.b <= 255 &&
			instruction.c <= 255;
	}

	std::string string() override {
		return "Stack[Instr[1]][Stack[Instr[2]]] = Stack[Instr[3]]";
	}
};

class vop_settable_bc_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SETTABLE &&
			instruction.b > 255 &&
			instruction.c <= 255;
	}

	std::string string() override {
		return "Stack[Instr[1]][Constants[Instr[2] - 256]] = Stack[Instr[3]]";
	}
};

class vop_settable_bc_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SETTABLE &&
			instruction.b <= 255 &&
			instruction.c > 255;
	}

	std::string string() override {
		return "Stack[Instr[1]][Stack[Instr[2]]] = Constants[Instr[3] - 256]";
	}
};

class vop_settable_bc_4_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SETTABLE &&
			instruction.b > 255 &&
			instruction.c > 255;
	}

	std::string string() override {
		return "Stack[Instr[1]][Constants[Instr[2] - 256]] = Constants[Instr[3] - 256]";
	}
};

class vmut_settable_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine ) override {
		switch ( rand_engine() % 6 ) {
			case 0: return new vop_settable_1_t();
			case 1: return new vop_settable_2_t();
			case 2: return new vop_settable_bc_1_t();
			case 3: return new vop_settable_bc_2_t();
			case 4: return new vop_settable_bc_3_t();
			case 5: return new vop_settable_bc_4_t();
		}

		return nullptr;
	}
};

#endif
