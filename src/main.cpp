#include <windows.h>
#include <processthreadsapi.h>

#include <iostream>
#include <fstream>
#include <filesystem>

#include "deserializer/deserializer.hpp"
#include "deserializer/chunk.hpp"

namespace fs = std::filesystem;

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

	std::ofstream outfile( outfile_p.c_str(), std::ios::out );
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

	std::ifstream luacfile( luacfile_p.c_str() );
	if ( !luacfile ) {
		std::cerr << "Error: Could not open luac file for deserialization" << std::endl;
		throw "ACCESS_DENIED_L";
	}

	chunk_t tl_chunk = decode_file( luacfile );
	luacfile.close();


	std::cout << "Obfuscating...\n\n";

	outfile.close();
}
