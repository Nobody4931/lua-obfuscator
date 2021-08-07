#pragma once
#ifndef OP_TFORLOOP_HEADER_HPP
#define OP_TFORLOOP_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_tforloop_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TFORLOOP;
	}

	std::string string() override {
		return R"(local A, C = Instr[1], Instr[3]

		local Returns = { Stack[A]( Stack[A + 1], Stack[A + 2] ) }
		for Idx = 1, C do
			Stack[A + 2 + Idx] = Returns[Idx]
		end

		if Stack[A + 3] == nil then
			InstrPtr = InstrPtr + 1
		else
			Stack[A + 2] = Stack[A + 3]
		end)";
	}
};

class vop_tforloop_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_TFORLOOP;
	}

	std::string string() override {
		return R"(local A, Offset = Instr[1], Instr[1] + 2

		local Returns = { Stack[A]( Stack[1 + A], Stack[A - -2] ) }
		for Idx = 1, Instr[3] do
			Stack[Idx + Offset] = Returns[Idx]
		end

		Offset = Stack[Offset + 1]
		if Offset ~= nil then
			Stack[2 - -A] = Offset
		else
			InstrPtr = 1 + InstrPtr
		end)";
	}
};

class vmut_tforloop_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		switch ( rand_engine() % 2 ) {
			case 0: return new vop_tforloop_1_t();
			case 1: return new vop_tforloop_2_t();
		}

		return nullptr;
	}
};

#endif
