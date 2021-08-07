#pragma once
#ifndef OP_VARARG_HEADER_HPP
#define OP_VARARG_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_vararg_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_VARARG;
	}

	std::string string() override {
		return R"(local A, B = Instr[1], Instr[2]
		for Offset = 0, B ~= 0 and B - 1 or #Varargs - 1 do
			Stack[A + Offset] = Varargs[Offset]
		end)";
	}
};

class vop_vararg_b_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_VARARG &&
			instruction.b == 0;
	}

	std::string string() override {
		return R"(local A = Instr[1]
		for Offset = 0, #Varargs + -1 do
			Stack[Offset + A] = Varargs[Offset]
		end)";
	}
};

class vop_vararg_b_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_VARARG &&
			instruction.b == 2;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Varargs[0]";
	}
};

class vop_vararg_b_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_VARARG &&
			instruction.b != 0;
	}

	std::string string() override {
		return R"(for Offset = 0, -1 + Instr[2] do
			Stack[Instr[1] - -Offset] = Varargs[Offset]
		end)";
	}
};

class vmut_vararg_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		switch ( rand_engine() % ( base_case ? 1 : 4 ) ) {
			case 0: return new vop_vararg_t();
			case 1: return new vop_vararg_b_1_t();
			case 2: return new vop_vararg_b_2_t();
			case 3: return new vop_vararg_b_3_t();
		}

		return nullptr;
	}
};

#endif
