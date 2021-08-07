#pragma once
#ifndef OP_TEST_HEADER_HPP
#define OP_TEST_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_test_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TEST;
	}

	std::string string() override {
		return R"(if Instr[3] == 0 then
			if Stack[Instr[1]] then
				InstrPtr = InstrPtr + 1
			end
		else
			if not Stack[Instr[1]] then
				InstrPtr = InstrPtr + 1
			end
		end)";
	}
};

class vop_test_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TEST;
	}

	std::string string() override {
		return R"(if (Instr[3] == 0 and Stack[Instr[1]]) or (not Stack[Instr[1]] and Instr[3] ~= 0) then
			InstrPtr = 1 + InstrPtr
		end)";
	}
};

class vop_test_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TEST;
	}

	std::string string() override {
		return "InstrPtr = ((Instr[3] == 0 and Stack[Instr[1]]) or (not Stack[Instr[1]] and Instr[3] ~= 0)) and 1 + InstrPtr or InstrPtr";
	}
};

class vop_test_c_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TEST &&
			instruction.c == 0;
	}

	std::string string() override {
		return "InstrPtr = InstrPtr + (Stack[Instr[1]] and 1 or 0)";
	}
};

class vop_test_c_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TEST &&
			instruction.c == 0;
	}

	std::string string() override {
		return "InstrPtr = InstrPtr + (not Stack[Instr[1]] and 0 or 1)";
	}
};

class vop_test_c_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TEST &&
			instruction.c != 0;
	}

	std::string string() override {
		return "InstrPtr = InstrPtr + (Stack[Instr[1]] and 0 or 1)";
	}
};

class vop_test_c_4_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TEST &&
			instruction.c != 0;
	}

	std::string string() override {
		return "InstrPtr = InstrPtr + (not Stack[Instr[1]] and 1 or 0)";
	}
};

class vmut_test_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		switch ( rand_engine() % ( base_case ? 3 : 7 ) ) {
			case 0: return new vop_test_1_t();
			case 1: return new vop_test_2_t();
			case 2: return new vop_test_3_t();
			case 3: return new vop_test_c_1_t();
			case 4: return new vop_test_c_2_t();
			case 5: return new vop_test_c_3_t();
			case 6: return new vop_test_c_4_t();
		}

		return nullptr;
	}
};

#endif
