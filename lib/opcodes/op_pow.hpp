#pragma once
#ifndef OP_POW_HEADER_HPP
#define OP_POW_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_pow_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_POW;
	}

	std::string string() override {
		return R"(local KeyB, KeyC = (function(...)
			local Keys = {...}
			for Idx = 1, #Keys do
				if Instr[Keys[Idx]] < 256 then
					Keys[Idx] = Stack[ Instr[Keys[Idx]] ]
				else
					Keys[Idx] = Constants[ Instr[Keys[Idx]] - 256 ]
				end
			end
			return Unpack(Keys)
		end)(2, 3)
		Stack[Instr[1]] = KeyB ^ KeyC)";
	}
};

class vop_pow_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_POW;
	}

	std::string string() override {
		return R"(Stack[Instr[1]] =
			( Instr[2] >= 256 and Constants[Instr[2] - 256] or Stack[Instr[2]] ) ^
			( Instr[3] > 255 and Constants[Instr[3] - 256] or Stack[Instr[3]] ))";
	}
};

class vop_pow_bc_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_POW &&
			instruction.b <= 255 &&
			instruction.c <= 255;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Stack[Instr[2]] ^ Stack[Instr[3]]";
	}
};

class vop_pow_bc_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_POW &&
			instruction.b > 255 &&
			instruction.c <= 255;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Constants[Instr[2] - 256] ^ Stack[Instr[3]]";
	}
};

class vop_pow_bc_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_POW &&
			instruction.b <= 255 &&
			instruction.c > 255;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Stack[Instr[2]] ^ Constants[Instr[3] - 256]";
	}
};

class vop_pow_bc_4_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_POW &&
			instruction.b > 255 &&
			instruction.c > 255;
	}

	std::string string() override {
		return "Stack[Instr[1]] = Constants[Instr[2] - 256] ^ Constants[Instr[3] - 256]";
	}
};

class vmut_pow_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		switch ( rand_engine() % ( base_case ? 2 : 6 ) ) {
			case 0: return new vop_pow_1_t();
			case 1: return new vop_pow_2_t();
			case 2: return new vop_pow_bc_1_t();
			case 3: return new vop_pow_bc_2_t();
			case 4: return new vop_pow_bc_3_t();
			case 5: return new vop_pow_bc_4_t();
		}

		return nullptr;
	}
};

#endif
