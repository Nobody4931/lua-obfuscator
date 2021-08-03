#pragma once
#ifndef OP_CLOSE_HEADER_HPP
#define OP_CLOSE_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_close_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CLOSE;
	}

	std::string string() override {
		return R"(local Closed = {}

		for Idx = 1, #LUpvalues do
			local Locals = LUpvalues[Idx]

			for Idz = 1, #Locals do
				local LUpvalue = Locals[Idz]
				local Memory = LUpvalue[1]
				local Register = LUpvalue[2]

				if Memory == Stack and Instr[1] <= Register then
					LUpvalue[1] = Closed
					Closed[Register] = Stack[Register]
				end
			end
		end)";
	}
};

class vmut_close_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine ) override {
		return new vop_close_t();
	}
};

#endif
