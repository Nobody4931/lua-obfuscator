#pragma once
#ifndef SERIALIZER_HEADER_HPP
#define SERIALIZER_HEADER_HPP

#include "bytecode/chunk.hpp"
#include "vm/generator.hpp"


void serialize_chunk( obfuscation_context_t& context, chunk_t& chunk, bool little_endian = true );

#endif
