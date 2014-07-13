add_library(solvercreator SHARED generator.cpp)
target_link_libraries(solvercreator ${REQ_LLVM_LIBRARIES})
