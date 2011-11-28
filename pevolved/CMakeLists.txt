cmake_minimum_required(VERSION 2.8)

include_directories(
	include
	${Addons_SOURCE_DIR}/include
)

file(
	GLOB_RECURSE
	srcs
	src/*.cpp

)

file(
	GLOB_RECURSE
	incs
	include/*.hpp
)

add_executable(
	pevolved
	${srcs}
	${incs}

)
