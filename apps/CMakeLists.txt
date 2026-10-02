foreach(app	bench_limits	bench_alloc	bench_matrix	bench_layout)
    add_executable(${app}	${app}.cpp)
    target_link_libraries(${app}	PRIVATE	matrix)
endforeach()