cmake_minimum_required(VERSION 2.8)

project(pevolved)

include_directories(
	include
	../addons/include
	${PROJECT_BINARY_DIR}
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


target_link_libraries(pevolved addons)
target_link_libraries(pevolved sfml-graphics sfml-window sfml-system)
