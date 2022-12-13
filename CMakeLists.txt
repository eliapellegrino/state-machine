cmake_minimum_required(VERSION 3.16)
include(cmake/arm-none-eabi-gcc.cmake)
# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/arm-none-eabi-gcc.cmake)
message(STATUS "CMAKE_TOOLCHAIN_FILE is: ${CMAKE_TOOLCHAIN_FILE}")

project(fsm_example 
VERSION 1.0
LANGUAGES C)

add_subdirectory(fsm_lib/)


# add_executable(example1
# main.c
# Motor.c
# CentrifugeTest.c
# )

# add_dependencies(example1 fsm)
# target_link_libraries(example1
# PRIVATE
# fsm)

add_executable(example1
    main2.c
)

add_dependencies(example1 fsm)


target_compile_options(example1 PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard

        -fdata-sections
        -ffunction-sections

        -Wall
        -O0
        -g3
        )

# Linker options
target_link_options(example1 PRIVATE
        -T${LINKER_FILE}
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -specs=nano.specs
        -lc
        -lm
        -lnosys
        -Wl,-Map=example1.map,--cref
        -Wl,--gc-sections
        -Xlinker -print-memory-usage -Xlinker
        ) 

        target_link_libraries(example1
        PRIVATE
        fsm)