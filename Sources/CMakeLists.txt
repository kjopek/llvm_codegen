add_executable(Creator main.cpp generator.cpp)
target_link_libraries(Creator ${REQ_LLVM_LIBRARIES})
