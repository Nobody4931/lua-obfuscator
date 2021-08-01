#pragma once
#ifndef OP_FORLOOP_HEADER_HPP
#define OP_FORLOOP_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_forloop_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_FORLOOP;
	}

	std::string string() override {
		return R"(local A = Instr[1]
		local Step = Stack[A + 2]
		local Limit = Stack[A + 1]
		local SBX = Instr[2]

		Stack[A] = Stack[A] + Step
		if Step > 0 then
			if Stack[A] <= Limit then
				InstrPtr = InstrPtr + SBX
				Stack[A + 3] = Stack[A]
			end
		elseif Stack[A] >= Limit then
			InstrPtr = InstrPtr + SBX
			Stack[A + 3] = Stack[A]
		end)";
	}
};

class vop_forloop_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_FORLOOP;
	}

	std::string string() override {
		return R"(local SBX, A = Instr[2], Instr[1]
		Stack[A] = Stack[A + 2] + Stack[A]
		local LoopVar = Stack[A]
		if Abs(Stack[1 + A]) >= Abs(LoopVar) then
			Stack[A + 3] = LoopVar
			InstrPtr = SBX + InstrPtr
		end)";
	}
};

class vmut_forloop_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine ) override {
		switch ( rand_engine() % 2 ) {
			case 0: return new vop_forloop_1_t();
			case 1: return new vop_forloop_2_t();
		}

		return nullptr;
	}
};

#endif
