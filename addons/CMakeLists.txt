cmake_minimum_required(VERSION 2.8)

include_directories(include)

add_library(
	addons
	src/guiManager.cpp
	src/guiManager_Widget.cpp
)
