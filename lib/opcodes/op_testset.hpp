#pragma once
#ifndef OP_TESTSET_HEADER_HPP
#define OP_TESTSET_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_testset_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TESTSET;
	}

	std::string string() override {
		return R"(if Instr[3] ~= 0 then
			if Stack[Instr[2]] then
				Stack[Instr[1]] = Stack[Instr[2]]
			else
				InstrPtr = InstrPtr + 1
			end
		else
			if not Stack[Instr[2]] then
				Stack[Instr[1]] = Stack[Instr[2]]
			else
				InstrPtr = InstrPtr + 1
			end
		end)";
	}
};

class vop_testset_c_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TESTSET &&
			instruction.c == 0;
	}

	std::string string() override {
		return R"(local B = Stack[Instr[2]]
		if B then
			InstrPtr = 1 + InstrPtr
		else
			Stack[Instr[1]] = B
		end)";
	}
};

class vop_testset_c_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TESTSET &&
			instruction.c == 0;
	}

	std::string string() override {
		return R"(local B = Stack[Instr[2]]
		if not B then
			Stack[Instr[1]] = B
		else
			InstrPtr = 1 + InstrPtr
		end)";
	}
};

class vop_testset_c_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TESTSET &&
			instruction.c != 0;
	}

	std::string string() override {
		return R"(local B = Stack[Instr[2]]
		if not B then
			InstrPtr = 1 + InstrPtr
		else
			Stack[Instr[1]] = B
		end)";
	}
};

class vop_testset_c_4_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TESTSET &&
			instruction.c != 0;
	}

	std::string string() override {
		return R"(local B = Stack[Instr[2]]
		if B then
			Stack[Instr[1]] = B
		else
			InstrPtr = 1 + InstrPtr
		end)";
	}
};

class vmut_testset_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		switch ( rand_engine() % ( base_case ? 1 : 5 ) ) {
			case 0: return new vop_testset_t();
			case 1: return new vop_testset_c_1_t();
			case 2: return new vop_testset_c_2_t();
			case 3: return new vop_testset_c_3_t();
			case 4: return new vop_testset_c_4_t();
		}

		return nullptr;
	}
};

#endif
