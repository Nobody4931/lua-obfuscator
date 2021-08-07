#pragma once
#ifndef OP_TAILCALL_HEADER_HPP
#define OP_TAILCALL_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_tailcall_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TAILCALL;
	}

	std::string string() override {
		return "return Stack[Instr[1]]( Unpack( "
			"Stack, "
			"Instr[1] + 1, "
			"Instr[2] == 0 and "
				"nil or "
				"Instr[1] + Instr[2] - 1 "
		") )";
	}
};

class vop_tailcall_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TAILCALL;
	}

	std::string string() override {
		return R"(local B, A = Instr[2], Instr[1]
		if B == 1 then
			return Stack[A]()
		elseif B == 0 then
			return Stack[A]( Unpack( Stack, A + 1 ) )
		else
			return Stack[A]( Unpack( Stack, 1 + A, B + -1 - -A ) )
		end)";
	}
};

class vop_tailcall_b_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TAILCALL &&
			instruction.b == 0;
	}

	std::string string() override {
		return "return Stack[Instr[1]]( Unpack( Stack, 1 + Instr[1] ) )";
	}
};

class vop_tailcall_b_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TAILCALL &&
			instruction.b == 1;
	}

	std::string string() override {
		return "return Stack[Instr[1]]()";
	}
};

class vop_tailcall_b_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TAILCALL &&
			instruction.b >= 2;
	}

	std::string string() override {
		return R"(local A = Instr[1]
		return Stack[A]( Unpack( Stack, A - -1, -1 + Instr[2] + A ) ))";
	}
};

class vmut_tailcall_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		switch ( rand_engine() % ( base_case ? 2 : 5 ) ) {
			case 0: return new vop_tailcall_1_t();
			case 1: return new vop_tailcall_2_t();
			case 2: return new vop_tailcall_b_1_t();
			case 3: return new vop_tailcall_b_2_t();
			case 4: return new vop_tailcall_b_3_t();
		}

		return nullptr;
	}
};

#endif
