#pragma once
#ifndef OP_CALL_HEADER_HPP
#define OP_CALL_HEADER_HPP

#include <random>
#include <string>

#include "bytecode/instruction.hpp"
#include "opcodes/vopcode.hpp"

class vop_call_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CALL;
	}

	std::string string() override {
		return R"(local A = Instr[1]
		local Returns = { Stack[A]( Unpack( Stack, A + 1, Instr[2] ~= 0 and A + Instr[2] - 1 or nil ) ) }
		for Idx = 1, Instr[3] == 0 and #Returns or Instr[3] - 1 do
			Stack[A + Idx - 1] = Returns[Idx]
		end)";
	}
};

class vop_call_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CALL;
	}

	std::string string() override {
		return R"(local Offset, B, C, Returns = 0, Instr[2], Instr[3]
		if B == 0 then
			Returns = { Stack[Instr[1]]( Unpack( Stack, 1 + Instr[1] ) ) }
		else
			Returns = { Stack[Instr[1]]( Unpack( Stack, Instr[1] + 1, -1 + B + Instr[1] ) ) }
		end
		C = C ~= 0 and C - 1 or #Returns
		repeat
			Stack[Offset + Instr[1]] = Returns[Offset + 1]
			Offset = 1 + Offset
		until Offset >= C)";
	}
};

class vop_call_bc_1_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CALL &&
			instruction.b == 0 &&
			instruction.c == 0;
	}

	std::string string() override {
		return R"(local A = Instr[1]
		local Returns = { Stack[A]( Unpack( Stack, 1 + A ) ) }
		for Idx = 1, #Returns do
			Stack[A + -1 + Idx] = Returns[Idx]
		end)";
	}
};

class vop_call_bc_2_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CALL &&
			instruction.b == 1 &&
			instruction.c == 0;
	}

	std::string string() override {
		return R"(local Returns, Offset = { Stack[Instr[1]]() }, 1
		while Offset < 1 + #Returns do
			Stack[Offset - 1 + Instr[1]] = Returns[Offset]
			Offset = Offset + 1
		end)";
	}
};

class vop_call_bc_3_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CALL &&
			instruction.b >= 2 &&
			instruction.c == 0;
	}

	std::string string() override {
		return R"(local Offset, A = 0, Instr[1]
		local Returns = { Stack[A]( Unpack( Stack, A + 1, Instr[2] - 1 + A ) ) }
		repeat
			Offset = Offset + 1
			Stack[Offset + A + -1] = Returns[Offset]
		until -1 + #Returns < Offset)";
	}
};

class vop_call_bc_4_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CALL &&
			instruction.b == 0 &&
			instruction.c == 1;
	}

	std::string string() override {
		return "Stack[Instr[1]]( Unpack( Stack, Instr[1] + 1 ) )";
	}
};

class vop_call_bc_5_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CALL &&
			instruction.b == 1 &&
			instruction.c == 1;
	}

	std::string string() override {
		return "Stack[Instr[1]]()";
	}
};

class vop_call_bc_6_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CALL &&
			instruction.b >= 2 &&
			instruction.c == 1;
	}

	std::string string() override {
		return R"(local A = Instr[1]
		Stack[A]( Unpack( Stack, 1 + A, -1 + A + Instr[2] ) ))";
	}
};

class vop_call_bc_7_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CALL &&
			instruction.b == 0 &&
			instruction.c >= 2;
	}

	std::string string() override {
		return R"(local Returns = { Stack[Instr[1]]( Unpack( Stack, 1 + Instr[1] ) ) }
		for Idx = 1, Instr[3] - 1 do
			Stack[Idx - 1 + Instr[1]] = Returns[Idx]
		end)";
	}
};

class vop_call_bc_8_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CALL &&
			instruction.b == 1 &&
			instruction.c >= 2;
	}

	std::string string() override {
		return R"(local Returns = { Stack[Instr[1]]() }
		for Idx = 1, Instr[3] - 1 do
			Stack[Idx + Instr[1] + -1] = Returns[Idx]
		end)";
	}
};

class vop_call_bc_9_t : public vopcode_t {
public:
	bool valid( instruction_t& instruction ) override {
		return instruction.opcode == OP_CALL &&
			instruction.b >= 2 &&
			instruction.c >= 2;
	}

	std::string string() override {
		return R"(local Returns, Offset = { Stack[Instr[1]]( Unpack( Stack, 1 + Instr[1], Instr[1] + -1 + Instr[2] ) ) }, 1
		while Instr[3] > Offset do
			Stack[Instr[1] + Offset - 1] = Returns[Offset]
			Offset = 1 + Offset
		end)";
	}
};

class vmut_call_t : public vmutator_t {
public:
	vopcode_t* mutate( std::default_random_engine& rand_engine, bool base_case ) override {
		switch ( rand_engine() % ( base_case ? 2 : 11 ) ) {
			case 0:  return new vop_call_1_t();
			case 1:  return new vop_call_2_t();
			case 2:  return new vop_call_bc_1_t();
			case 3:  return new vop_call_bc_2_t();
			case 4:  return new vop_call_bc_3_t();
			case 5:  return new vop_call_bc_4_t();
			case 6:  return new vop_call_bc_5_t();
			case 7:  return new vop_call_bc_6_t();
			case 8:  return new vop_call_bc_7_t();
			case 9:  return new vop_call_bc_8_t();
			case 10: return new vop_call_bc_9_t();
		}

		return nullptr;
	}
};

#endif
