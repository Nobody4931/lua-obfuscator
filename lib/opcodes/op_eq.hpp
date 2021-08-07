#pragma once
#ifndef OP_EQ_HEADER_HPP
#define OP_EQ_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_eq_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_EQ;
	}

	std::string string() override {
		return R"(local KeyB, KeyC =
			Instr[2] < 256 and Stack[Instr[2]] or Constants[Instr[2] - 256],
			Instr[3] > 255 and Constants[Instr[3] - 256] or Stack[Instr[3]]
		if (KeyB == KeyC) ~= (Instr[1] ~= 0) then
			InstrPtr = InstrPtr + 1
		end)";
	}
};

class vop_eq_abc_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_EQ &&
			instruction.a == 0 &&
			instruction.b <= 255 &&
			instruction.c <= 255;
	}

	std::string string() override {
		return "InstrPtr = InstrPtr + (not (Stack[Instr[2]] ~= Stack[Instr[3]]) and 1 or 0)";
	}
};

class vop_eq_abc_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_EQ &&
			instruction.a == 0 &&
			instruction.b > 255 &&
			instruction.c <= 255;
	}

	std::string string() override {
		return "InstrPtr = InstrPtr + (Constants[Instr[2] - 256] ~= Stack[Instr[3]] and 0 or 1)";
	}
};

class vop_eq_abc_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_EQ &&
			instruction.a == 0 &&
			instruction.b <= 255 &&
			instruction.c > 255;
	}

	std::string string() override {
		return "InstrPtr = not (Stack[Instr[2]] == Constants[Instr[3] - 256]) and InstrPtr or 1 + InstrPtr";
	}
};

class vop_eq_abc_4_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_EQ &&
			instruction.a == 0 &&
			instruction.b > 255 &&
			instruction.c > 255;
	}

	std::string string() override {
		return "InstrPtr = Constants[Instr[2] - 256] == Constants[Instr[3] - 256] and 1 + InstrPtr or InstrPtr";
	}
};

class vop_eq_abc_5_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_EQ &&
			instruction.a != 0 &&
			instruction.b <= 255 &&
			instruction.c <= 255;
	}

	std::string string() override {
		return "InstrPtr = not (Stack[Instr[2]] ~= Stack[Instr[3]]) and InstrPtr or InstrPtr + 1";
	}
};

class vop_eq_abc_6_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_EQ &&
			instruction.a != 0 &&
			instruction.b > 255 &&
			instruction.c <= 255;
	}

	std::string string() override {
		return "InstrPtr = Constants[Instr[2] - 256] ~= Stack[Instr[3]] and 1 + InstrPtr or InstrPtr";
	}
};

class vop_eq_abc_7_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_EQ &&
			instruction.a != 0 &&
			instruction.b <= 255 &&
			instruction.c > 255;
	}

	std::string string() override {
		return "InstrPtr = InstrPtr + (not (Stack[Instr[2]] ~= Constants[Instr[3] - 256]) and 0 or 1)";
	}
};

class vop_eq_abc_8_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_EQ &&
			instruction.a != 0 &&
			instruction.b > 255 &&
			instruction.c > 255;
	}

	std::string string() override {
		return "InstrPtr = InstrPtr + (Constants[Instr[2] - 256] ~= Constants[Instr[3] - 256] and 1 or 0)";
	}
};

class vmut_eq_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine &rand_engine, bool base_case ) override {
		switch ( rand_engine() % ( base_case ? 1 : 9 ) ) {
			case 0: return new vop_eq_t();
			case 1: return new vop_eq_abc_1_t();
			case 2: return new vop_eq_abc_2_t();
			case 3: return new vop_eq_abc_3_t();
			case 4: return new vop_eq_abc_4_t();
			case 5: return new vop_eq_abc_5_t();
			case 6: return new vop_eq_abc_6_t();
			case 7: return new vop_eq_abc_7_t();
			case 8: return new vop_eq_abc_8_t();
		}

		return nullptr;
	}
};

#endif
