#include <windows.h>
#include <processthreadsapi.h>

#include <iostream>
#include <fstream>
#include <filesystem>

#include "bytecode/deserializer.hpp"
#include "bytecode/chunk.hpp"

#define DEBUG_MODE 1

namespace fs = std::filesystem;


#if DEBUG_MODE == 1

static const char* instruction_names[] {
	"MOVE",
	"LOADK",
	"LOADBOOL",
	"LOADNIL",
	"GETUPVAL",
	"GETGLOBAL",
	"GETTABLE",
	"SETGLOBAL",
	"SETUPVAL",
	"SETTABLE",
	"NEWTABLE",
	"SELF",
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"MOD",
	"POW",
	"UNM",
	"NOT",
	"LEN",
	"CONCAT",
	"JMP",
	"EQ",
	"LT",
	"LE",
	"TEST",
	"TESTSET",
	"CALL",
	"TAILCALL",
	"RETURN",
	"FORLOOP",
	"FORPREP",
	"TFORLOOP",
	"SETLIST",
	"CLOSE",
	"CLOSURE",
	"VARARG",
};

static inline void indent( uint16_t depth ) {
	for ( uint16_t i = 0; i < depth; ++i ) {
		std::cout << "    "; // why the fuck is a tab character 8 spaces
	}
}

static inline void output_chunk( chunk_t& chunk, uint16_t depth = 0 ) {
	indent( depth );
	std::cout << "Constants:\n";
	for ( l_int i = 0; i < chunk.constant_cnt; ++i ) {
		indent( depth );
		std::cout << '[' << i << "]: ";
		switch ( chunk.constants[i].type ) {
			case const_t::K_NIL:
				std::cout << "nil";
				break;

			case const_t::K_BOOLEAN:
				std::cout << ( *chunk.constants[i].data ? "true" : "false" );
				break;

			case const_t::K_NUMBER:
				std::cout << *reinterpret_cast<l_number*>( chunk.constants[i].data );
				break;

			case const_t::K_STRING:
				std::cout << '"' << reinterpret_cast<l_string*>( chunk.constants[i].data )->data << '"';
				break;
		}
		std::cout << '\n';
	}
	std::cout << '\n';

	indent( depth );
	std::cout << "Functions:\n";
	for ( l_int i = 0; i < chunk.function_cnt; ++i ) {
		indent( depth );
		std::cout << '[' << i << "]:\n";
		output_chunk( chunk.functions[i], depth + 1 );
	}
	std::cout << '\n';

	indent( depth );
	std::cout << "Instructions:\n";
	for ( l_int i = 0; i < chunk.instruction_cnt; ++i ) {
		indent( depth );
		std::cout << '[' << i << "]: ";
		std::cout << instruction_names[ chunk.instructions[i].opcode ] << ' ';
		// fuck you lua
		switch ( chunk.instructions[i].type ) {
			case instr_t::i_ABC:
				std::cout
					<< chunk.instructions[i].a << ' '
					<< chunk.instructions[i].b << ' '
					<< chunk.instructions[i].c;
				break;

			case instr_t::i_ABx:
			case instr_t::i_AsBx:
				std::cout
					<< chunk.instructions[i].a << ' '
					<< chunk.instructions[i].b;
				break;
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

#endif // DEBUG_MODE == 1

int main( int argc, char** argv ) {
	if ( argc != 3 ) {
		std::cerr << "Usage: lua_obfuscator.exe <input> <output>" << std::endl;
		return 1;
	}

	fs::path infile_p = argv[1];
	fs::path outfile_p = argv[2];

	if ( !fs::is_regular_file( infile_p ) ) {
		std::cerr << "Error: Provided input path is not a file" << std::endl;
		throw "INVALID_PATH";
	}

	std::ofstream outfile( outfile_p.c_str(), std::ofstream::out );
	if ( !outfile ) {
		std::cerr << "Error: Could not open output file for reading" << std::endl;
		throw "ACCESS_DENIED_O";
	}


	std::cout << "Compiling...\n\n";

	fs::path luacfile_p = outfile_p;
	luacfile_p.replace_extension("luac");

	LPCSTR luac_path = R"(C:\Users\arden\Documents\Programming\Projects\C++\lua_obfuscator\bin\src\luac.exe)";
	std::string luac_args_tmp = std::string( "luac.exe" ) + " -o " + luacfile_p.string() + " " + infile_p.string();
	LPSTR luac_args = luac_args_tmp.data();

	STARTUPINFOA luac_sinfo;
	ZeroMemory( &luac_sinfo, sizeof( luac_sinfo ) );
	luac_sinfo.cb = sizeof( luac_sinfo );

	PROCESS_INFORMATION luac_pinfo;
	ZeroMemory( &luac_pinfo, sizeof( luac_pinfo ) );

	if ( !CreateProcessA(
				luac_path,
				luac_args,
				NULL, NULL, false,
				0,
				NULL,
				NULL,
				&luac_sinfo,
				&luac_pinfo ) ) {
		std::cerr << "Error: Failed to launch compiler" << std::endl;
		throw "PROCESS_START_FAILED";
	}

	WaitForSingleObject( luac_pinfo.hProcess, INFINITE );
	CloseHandle( luac_pinfo.hProcess );
	CloseHandle( luac_pinfo.hThread );

	if ( !fs::is_regular_file( luacfile_p ) ) {
		throw "COMPILATION_FAILED";
	}


	std::cout << "Deserializing...\n\n";

	std::ifstream luacfile( luacfile_p.c_str(), std::ifstream::in | std::ifstream::binary );
	if ( !luacfile ) {
		std::cerr << "Error: Could not open luac file for deserialization" << std::endl;
		throw "ACCESS_DENIED_L";
	}

	chunk_t tl_chunk = decode_file( luacfile );
	luacfile.close();

#if DEBUG_MODE == 1
	output_chunk( tl_chunk );
#endif


	std::cout << "Obfuscating...\n\n";

	outfile.close();
}
