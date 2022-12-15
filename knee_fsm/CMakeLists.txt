project(knee_fsm
VERSION 1.0
LANGUAGES C)

set(lib_name    ${PROJECT_NAME})
add_library(${lib_name}
    STATIC
    src/StateMachine.c
    src/Knee_Fsm.c
)

target_include_directories(${lib_name}
    PUBLIC
    include/
)

#Compiler options
# target_compile_options(${lib_name} PRIVATE
#         -mcpu=cortex-m4
#         -mthumb
#         -mfpu=fpv4-sp-d16
#         -mfloat-abi=hard

#         -fdata-sections
#         -ffunction-sections

#         -Wall
#         -O0
#         -g3
#         )

# # Linker options
target_link_options(${lib_name} PRIVATE
        -Wl, --print-memory-usage
        ) 