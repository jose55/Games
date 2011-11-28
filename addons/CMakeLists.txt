cmake_minimum_required(VERSION 2.8)

project(Addons)

include_directories(include)

file(
		GLOB_RECURSE
		srcs
		src/*
)

file(
	GLOB_RECURSE
	incs
	include/*
)

add_library(
	addons
	${srcs}
	${incs}
)
