#pragma once
#ifndef OP_CLOSURE_HEADER_HPP
#define OP_CLOSURE_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_closure_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CLOSURE;
	}

	std::string string() override {
		return R"(local Proto = Protos[Instr[2]]

		local Locals = {}
		local PUpvalues = Setmetatable({}, {
			[__Index] = function(_, Key)
				local LUpvalue = Locals[Key]
				return LUpvalue[1][LUpvalue[2]]
			end,

			[__NewIndex] = function(_, Key, Value)
				local LUpvalue = Locals[Key]
				LUpvalue[1][LUpvalue[2]] = Value
			end
		})

		for Idx = 1, Proto[2] do
			local PInstr = Instrs[InstrPtr]
			if IsMoveOp(PInstr) then
				Locals[Idx - 1] = { Stack, PInstr[2] }
			else
				Locals[Idx - 1] = { Upvalues, PInstr[2] }
			end

			InstrPtr = InstrPtr + 1
		end

		Stack[Instr[1]] = Wrap(Proto, Env, PUpvalues)
		LUpvalues[#LUpvalues + 1] = Locals)";
	}
};

class vop_closure_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CLOSURE;
	}

	std::string string() override {
		return R"(
		local Locals = {}
		local PUpvalues = {}

		local PUpvaluesMT = {
			[__NewIndex] = function(_, Key, Value)
				local LUpvalue = Locals[Key]
				LUpvalue[1][LUpvalue[2]] = Value
			end
		}

		PUpvaluesMT[__Index] = function(_, Key)
			local LUpvalue = Locals[Key]
			return LUpvalue[1][LUpvalue[2]]
		end

		Setmetatable(PUpvalues, PUpvaluesMT)

		local Proto = Protos[Instr[2]]
		for Idx = 1, Proto[2] do
			local PInstr = Instrs[InstrPtr]
			local Register = PInstr[2]

			if IsGetupvalOp(PInstr) then
				Locals[Register] = { Upvalues, -1 + Idx }
			else
				Locals[Idx + -1] = { Stack, Register }
			end

			InstrPtr = 1 + InstrPtr
		end

		LUpvalues[#LUpvalues - -1] = Locals
		Stack[Instr[1]] = Wrap(Proto, Env, PUpvalues))";
	}
};

class vmut_closure_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine ) override {
		switch ( rand_engine() % 2 ) {
			case 0: return new vop_closure_1_t();
			case 1: return new vop_closure_2_t();
		}

		return nullptr;
	}
};

#endif
