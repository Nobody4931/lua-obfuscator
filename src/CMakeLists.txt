list(APPEND PROJECT_LIBS
	"bytecode/deserializer.cpp"
	"bytecode/chunk.cpp"
	"bytecode/instruction.cpp"
	"bytecode/constant.cpp")

list(APPEND PROJECT_LIBS
	"vm/generator.cpp")

add_executable(${PROJECT_NAME}
	${PROJECT_LIBS}
	main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
	"${PROJECT_LIB_FOLDER}")
