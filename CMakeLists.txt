cmake_minimum_required(VERSION 3.0)

project(c-generic-containers)

set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -ansi -pedantic -Wold-style-definition -Wcast-qual -Wlogical-op -Wno-variadic-macros")
set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

include_directories(include)

file(
	GLOB_RECURSE
	source_files
	src/*
	include/*
)

add_library(
	containers
	SHARED
	${source_files}
)