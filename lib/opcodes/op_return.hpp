#pragma once
#ifndef OP_RETURN_HEADER_HPP
#define OP_RETURN_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_return_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_RETURN;
	}

	std::string string() override {
		return "return Unpack( "
			"Stack, "
			"Instr[1], "
			"Instr[2] ~= 0 and "
				"Instr[1] + Instr[2] - 2 or "
				"nil "
		")";
	}
};

class vop_return_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_RETURN;
	}

	std::string string() override {
		return R"(local B, A = Instr[2], Instr[1]
		if B == 1 then
			return
		elseif B == 0 then
			return Unpack( Stack, A )
		else
			return Unpack( Stack, A, A + -2 - -B )
		end)";
	}
};

class vop_return_b_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_RETURN &&
			instruction.b == 0;
	}

	std::string string() override {
		return "return Unpack( Stack, Instr[1] )";
	}
};

class vop_return_b_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_RETURN &&
			instruction.b == 1;
	}

	std::string string() override {
		return "return";
	}
};

class vop_return_b_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_RETURN &&
			instruction.b >= 2;
	}

	std::string string() override {
		return "return Unpack( Stack, Instr[1], Instr[2] - 1 + Instr[1] + -1 )";
	}
};

class vmut_return_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine ) override {
		switch ( rand_engine() % 5 ) {
			case 0: return new vop_return_1_t();
			case 1: return new vop_return_2_t();
			case 2: return new vop_return_b_1_t();
			case 3: return new vop_return_b_2_t();
			case 4: return new vop_return_b_3_t();
		}

		return nullptr;
	}
};

#endif
