#pragma once
#ifndef OP_LOADBOOL_HEADER_HPP
#define OP_LOADBOOL_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_loadbool_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADBOOL;
	}

	std::string string() override {
		return R"(Stack[Instr[1]] = Instr[2] ~= 0
		if Instr[3] ~= 0 then
			InstrPtr = InstrPtr + 1
		end)";
	}
};

class vop_loadbool_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADBOOL;
	}

	std::string string() override {
		return R"(Stack[Instr[1]] = not (Instr[2] == 0)
		InstrPtr = Instr[3] == 0 and InstrPtr or 1 + InstrPtr)";
	}
};

class vop_loadbool_bc_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADBOOL &&
			instruction.b != 0 &&
			instruction.c != 0;
	}

	std::string string() override {
		return R"(Stack[Instr[1]] = true
		InstrPtr = InstrPtr + 1)";
	}
};

class vop_loadbool_bc_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADBOOL &&
			instruction.b == 0 &&
			instruction.c != 0;
	}

	std::string string() override {
		return R"(Stack[Instr[1]] = false
		InstrPtr = 1 + InstrPtr)";
	}
};

class vop_loadbool_bc_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADBOOL &&
			instruction.b != 0 &&
			instruction.c == 0;
	}

	std::string string() override {
		return R"(Stack[Instr[1]] = not nil)";
	}
};

class vop_loadbool_bc_4_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADBOOL &&
			instruction.b == 0 &&
			instruction.c == 0;
	}

	std::string string() override {
		return R"(Stack[Instr[1]] = not true)";
	}
};


class vmut_loadbool_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		switch ( rand_engine() % ( base_case ? 2 : 6 ) ) {
			case 0: return new vop_loadbool_1_t();
			case 1: return new vop_loadbool_2_t();
			case 2: return new vop_loadbool_bc_1_t();
			case 3: return new vop_loadbool_bc_2_t();
			case 4: return new vop_loadbool_bc_3_t();
			case 5: return new vop_loadbool_bc_4_t();
		}

		return nullptr;
	}
};

#endif
