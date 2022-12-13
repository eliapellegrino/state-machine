project(fsm
VERSION 1.0
LANGUAGES C)

set(lib_name    ${PROJECT_NAME})
add_library(fsm
    STATIC
    src/StateMachine.c
    src/x_allocator.c
    src/sm_allocator.c
    src/fb_allocator.c
    src/Fault.c
    #src/LockGuard.c
    
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