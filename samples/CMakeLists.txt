set(PROJECT tests)


set(LIBS
	boolcalc
)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../include/
)

add_executable (sample_1 sample_1.cpp)

target_link_libraries (sample_1 ${LIBS})
