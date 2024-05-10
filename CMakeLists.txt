cmake_minimum_required(VERSION 3.16.3)

project(shader_recompiler)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

function(create_target_directory_groups target_name)
    source_group("Other Files" REGULAR_EXPRESSION ".")
    get_target_property(target_sources "${target_name}" SOURCES)
    foreach(file_name IN LISTS target_sources)
        get_filename_component(dir_name "${file_name}" PATH)
        string(REPLACE "/" "\\" group_name "${dir_name}")
        source_group("${group_name}" FILES "${file_name}")
    endforeach()
endfunction()

add_subdirectory(externals)

add_library(shader_recompiler STATIC
            src/exception.h
            src/object_pool.h
            src/common/assert.cpp
            src/common/assert.h
            src/common/bit_field.h
            src/common/enum.h
            src/common/types.h
            src/frontend/translate/data_share.cpp
            src/frontend/translate/export.cpp
            src/frontend/translate/flat_memory.cpp
            src/frontend/translate/scalar_alu.cpp
            src/frontend/translate/scalar_memory.cpp
            src/frontend/translate/translate.cpp
            src/frontend/translate/translate.h
            src/frontend/translate/vector_alu.cpp
            src/frontend/translate/vector_interpolation.cpp
            src/frontend/translate/vector_memory.cpp
            src/frontend/control_flow_graph.cpp
            src/frontend/control_flow_graph.h
            src/frontend/decode.cpp
            src/frontend/decode.h
            src/frontend/format.cpp
            src/frontend/instruction.h
            src/frontend/opcodes.h
            src/frontend/structured_control_flow.cpp
            src/frontend/structured_control_flow.h
            src/ir/passes/ssa_rewrite_pass.cpp
            src/ir/abstract_syntax_list.h
            src/ir/attribute.cpp
            src/ir/attribute.h
            src/ir/basic_block.cpp
            src/ir/basic_block.h
            src/ir/breadth_first_search.h
            src/ir/condition.h
            src/ir/ir_emitter.cpp
            src/ir/ir_emitter.h
            src/ir/microinstruction.cpp
            src/ir/opcodes.cpp
            src/ir/opcodes.h
            src/ir/opcodes.inc
            src/ir/post_order.cpp
            src/ir/post_order.h
            src/ir/program.cpp
            src/ir/program.h
            src/ir/reg.h
            src/ir/type.cpp
            src/ir/type.h
            src/ir/value.cpp
            src/ir/value.h
            src/frontend/instruction.cpp
)
target_include_directories(shader_recompiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(shader_recompiler PUBLIC fmt boost)
create_target_directory_groups(shader_recompiler)

add_subdirectory(tools/sb_parser)
