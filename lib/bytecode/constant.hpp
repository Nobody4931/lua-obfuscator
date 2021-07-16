#pragma once
#ifndef CONSTANT_HEADER_HPP
#define CONSTANT_HEADER_HPP

#include <cstdint>

#include "datatypes.hpp"

enum const_t : uint8_t {
	K_NIL     = 0,
	K_BOOLEAN = 1,
	K_NUMBER  = 3,
	K_STRING  = 4
};

struct constant_t {
	const_t type;
	uint8_t* data;
};

#endif
