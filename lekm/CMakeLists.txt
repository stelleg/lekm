include_directories(${VM_SRC_DIR}/lekm)
add_executable(lekm lekm.c trace.c)
target_link_libraries(lekm bytecode)
