#pragma once
#ifndef VOPCODE_HEADER_HPP
#define VOPCODE_HEADER_HPP

#include <string>

#include "bytecode/instruction.hpp"

class vopcode_t {
public:
	virtual bool valid( instruction_t& instruction ) = 0;
	virtual std::string string() = 0;
	virtual ~vopcode_t() { };
};

class vmutator_t {
public:
	virtual vopcode_t* mutate() = 0;
	virtual ~vmutator_t() { };
};

#endif
