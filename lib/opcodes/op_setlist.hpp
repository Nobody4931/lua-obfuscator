#pragma once
#ifndef OP_SETLIST_HEADER_HPP
#define OP_SETLIST_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_setlist_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SETLIST;
	}

	std::string string() override {
		return R"(local A, BlkIdx = Instr[1]
		if Instr[3] ~= 0 then
			BlkIdx = Instr[3]
		else
			BlkIdx = Instrs[InstrPtr][4]
			InstrPtr = InstrPtr + 1
		end

		BlkIdx = (BlkIdx - 1) * 50
		for Offset = 1, Instr[2] == 0 and #Stack - A or Instr[2] do
			Stack[A][BlkIdx + Offset] = Stack[A + Offset]
		end)";
	}
};

class vop_setlist_bc_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SETLIST &&
			instruction.b == 0 &&
			instruction.c == 0;
	}

	std::string string() override {
		return R"(local A, BlkIdx = Instr[1], 50 * (Instrs[InstrPtr][4] + -1)
		for Offset = 1, #Stack + -A do
			Stack[A][Offset + BlkIdx] = Stack[Offset + A]
		end
		InstrPtr = 1 + InstrPtr)";
	}
};

class vop_setlist_bc_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SETLIST &&
			instruction.b != 0 &&
			instruction.c == 0;
	}

	std::string string() override {
		return R"(local BlkIdx, B, A = Instrs[InstrPtr][4] * 50 + -50, Instr[2], Instr[1]
		InstrPtr = InstrPtr - -1
		for Offset = 1, Instr[2] do
			Stack[A][BlkIdx + Offset] = Stack[Offset + A]
		end)";
	}
};

class vop_setlist_bc_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SETLIST &&
			instruction.b == 0 &&
			instruction.c != 0;
	}

	std::string string() override {
		return R"(local A = Instr[1]
		local BlkIdx = 50 * Instr[3] - 50
		for Offset = 1, -A + #Stack do
			Stack[A][Offset + BlkIdx] = Stack[A + Offset]
		end)";
	}
};

class vop_setlist_bc_4_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_SETLIST &&
			instruction.b != 0 &&
			instruction.c != 0;
	}

	std::string string() override {
		return R"(local BlkIdx, A = 50 * (-1 + Instr[3]), Instr[1]
		for Offset = 1, Instr[2] do
			Stack[A][Offset - -BlkIdx] = Stack[Offset + A]
		end)";
	}
};

class vmut_setlist_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine ) override {
		switch ( rand_engine() % 5 ) {
			case 0: return new vop_setlist_t();
			case 1: return new vop_setlist_bc_1_t();
			case 2: return new vop_setlist_bc_2_t();
			case 3: return new vop_setlist_bc_3_t();
			case 4: return new vop_setlist_bc_4_t();
		}

		return nullptr;
	}
};

#endif
