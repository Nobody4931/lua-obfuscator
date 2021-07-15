list(APPEND PROJECT_LIBS
	"deserializer/deserializer.cpp"
	"deserializer/chunk.cpp"
	"deserializer/instruction.cpp"
	"deserializer/constant.cpp")

add_executable(${PROJECT_NAME}
	${PROJECT_LIBS}
	main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
	"${PROJECT_LIB_FOLDER}")
