cmake_minimum_required(VERSION 2.8)

include_directories(include)

file(
		GLOB_RECURSE
		srcs
		src/*
)

add_library(
	addons
	${srcs}
)
