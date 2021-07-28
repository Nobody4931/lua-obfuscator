#pragma once
#ifndef OP_MOD_HEADER_HPP
#define OP_MOD_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_mod_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_MOD;
	}

	std::string string() override {
		return R"(local KeyC, KeyB = (function(...)
			local Keys = {...}
			for Idx = 1, #Keys do
				Keys[Idx] = Instr[Keys[Idx]] > 255 and Constants[ Instr[Keys[Idx]] - 256 ] or Stack[ Instr[Keys[Idx]] ]
			end
			return Unpack(Keys)
		end)(3, 2)
		Stack[Instr[1]] = KeyB % KeyC)";
	}
};

class vop_mod_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_MOD;
	}

	std::string string() override {
		return R"(Stack[Instr[1]] =
			( Instr[2] > 255 and Constants[Instr[2] - 256] or Stack[Instr[2]] ) %
			( Instr[3] >= 256 and Constants[Instr[3] - 256] or Stack[Instr[3]] ))";
	}
};

class vop_mod_bc_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_MOD &&
			instruction.b <= 255 &&
			instruction.c <= 255;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Stack[Instr[2]] % Stack[Instr[3]]";
	}
};

class vop_mod_bc_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_MOD &&
			instruction.b > 255 &&
			instruction.c <= 255;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Constants[Instr[2] - 256] % Stack[Instr[3]]";
	}
};

class vop_mod_bc_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_MOD &&
			instruction.b <= 255 &&
			instruction.c > 255;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Stack[Instr[2]] % Constants[Instr[3] - 256]";
	}
};

class vop_mod_bc_4_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_MOD &&
			instruction.b > 255 &&
			instruction.c > 255;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Constants[Instr[2] - 256] % Constants[Instr[3] - 256]";
	}
};

class vmut_mod_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine ) override {
		switch ( rand_engine() % 6 ) {
			case 0: return new vop_mod_1_t();
			case 1: return new vop_mod_2_t();
			case 2: return new vop_mod_bc_1_t();
			case 3: return new vop_mod_bc_2_t();
			case 4: return new vop_mod_bc_3_t();
			case 5: return new vop_mod_bc_4_t();
		}

		return nullptr;
	}
};

#endif
