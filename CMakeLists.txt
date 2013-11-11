cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED BOOLCALC_BUILD_AS_PART_OF_PROJECT)
	set (BUILD_DIR
	        ${CMAKE_CURRENT_SOURCE_DIR}/build
	)

	if(NOT IS_DIRECTORY ${BUILD_DIR})
	        file (MAKE_DIRECTORY
	                ${BUILD_DIR}
	        )
	endif()
endif()

if(CMAKE_COMPILER_IS_GNUCXX )
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++0x)
endif()

set(EXECUTABLE_OUTPUT_PATH ${BUILD_DIR})

set (LIBRARY_OUTPUT_PATH ${BUILD_DIR})

add_subdirectory(src/)

if(NOT DEFINED BOOLCALC_BUILD_AS_PART_OF_PROJECT)
	add_subdirectory(test/)
	add_subdirectory(samples/)
	enable_testing ()
endif()
