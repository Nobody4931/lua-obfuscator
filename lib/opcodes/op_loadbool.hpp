#pragma once
#ifndef OP_LOADBOOL_HEADER_HPP
#define OP_LOADBOOL_HEADER_HPP

#include <string>
#include <random>
#include <ctime>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_loadbool_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADBOOL;
	}

	std::string string() override {
		return R"(Stack[Instr[1]] = Instr[2] ~= 0
		if (Instr[3] ~= 0) then
			InstrPtr = InstrPtr + 1
		end)";
	}
};

class vop_loadbool_2_t : public vopcode_t {
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADBOOL;
	}

	std::string string() override {
		return R"(Stack[Instr[1]] = not (Instr[2] == 0)
		InstrPtr = Instr[3] == 0 and InstrPtr or InstrPtr + 1)";
	}
};

class vop_loadbool_3_t : public vopcode_t {
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADBOOL;
	}

	std::string string() override {
		return R"(if (Instr[2] == 0) then
			Stack[Instr[1]] = false
		else
			Stack[Instr[1]] = true
		end
		InstrPtr = InstrPtr + (Instr[3] ~= 0 and 1 or 0))";
	}
};

class vop_loadbool_c1_t : public vopcode_t {
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADBOOL && instruction.c == 0;
	}

	std::string string() override {
		return R"(if (Instr[2] ~= 0) then
			Stack[Instr[1]] = true
		else
			Stack[Instr[1]] = false
		end)";
	}
};

class vop_loadbool_c2_t : public vopcode_t {
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADBOOL && instruction.c != 0;
	}

	std::string string() override {
		return R"(Stack[Instr[1]] = not not (Instr[2] ~= 0)
		InstrPtr = InstrPtr + 1)";
	}
};

class vop_loadbool_b1_t : public vopcode_t {
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_LOADBOOL && instruction.b != 0;
	}

	std::string string() override {
		return R"(Stack[Instr[1]] = not nil
		while (Instr[3] ~= 0) do
			InstrPtr = InstrPtr + 1
			break
		end)";
	}
};

class vmut_loadbool_t : public vmutator_t {
public:
	vopcode_t* mutate() override {
		switch ( std::default_random_engine( time( nullptr ) )() % 6 ) {
			case 0: return new vop_loadbool_1_t();
			case 1: return new vop_loadbool_2_t();
			case 2: return new vop_loadbool_3_t();
			case 3: return new vop_loadbool_c1_t();
			case 4: return new vop_loadbool_c2_t();
			case 5: return new vop_loadbool_b1_t();
		}

		return nullptr;
	}
};

#endif
